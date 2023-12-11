// Based on the implementation by Nigel Redmon
// http://www.earlevel.com/main/2013/06/01/envelope-generators/

#include "ADSREnvelope.h"

#include <math.h>

ADSREnvelope::ADSREnvelope(int SR, float attackRate, float decayRate, float sustainLevel,
                           float releaseRate, float targetRatioA, float targetRatioDR) : Envelope(SR),
                                                                                         state(State::IDLE) {
    setAttackRate(attackRate);
    setDecayRate(decayRate);
    setSustainLevel(sustainLevel);
    setReleaseRate(releaseRate);
    setTargetRatioA(targetRatioA);
    setTargetRatioDR(targetRatioDR);
}

float ADSREnvelope::calcCoef(float rate, float targetRatio) {
    return (rate <= 0) ? 0.0 : exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}

void ADSREnvelope::setAttackRate(float rate) {
    attackRate = rate * sampleRate;
    attackCoef = calcCoef(attackRate, targetRatioA);
    attackBase = (1.0 + targetRatioA) * (1.0 - attackCoef);
}

void ADSREnvelope::setDecayRate(float rate) {
    decayRate = rate * sampleRate;
    decayCoef = calcCoef(decayRate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
}

void ADSREnvelope::setSustainLevel(float level) {
    sustainLevel = level;
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
}

void ADSREnvelope::setReleaseRate(float rate) {
    releaseRate = rate * sampleRate;
    releaseCoef = calcCoef(releaseRate, targetRatioDR);
    releaseBase = -targetRatioDR * (1.0 - releaseCoef);
}

void ADSREnvelope::setTargetRatioA(float targetRatio) {
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    targetRatioA = targetRatio;
    attackCoef = calcCoef(attackRate, targetRatioA);
    attackBase = (1.0 + targetRatioA) * (1.0 - attackCoef);
}

void ADSREnvelope::setTargetRatioDR(float targetRatio) {
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    targetRatioDR = targetRatio;
    decayCoef = calcCoef(decayRate, targetRatioDR);
    releaseCoef = calcCoef(releaseRate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
    releaseBase = -targetRatioDR * (1.0 - releaseCoef);
}

bool ADSREnvelope::isIdle() {
    return state == State::IDLE;
}

void ADSREnvelope::startNote() {
    state = State::ATTACK;
    output = 0.0;
}

void ADSREnvelope::endNote() {
    if (state != State::IDLE) {
        state = State::RELEASE;
    }
}

float ADSREnvelope::tick() {
    switch (state) {
        case State::IDLE:
            output = 0.0;
            break;
        case State::ATTACK:
            output = attackBase + output * attackCoef;
            if (output >= 1.0) {
                output = 1.0;
                state = State::DECAY;
            }
            break;
        case State::DECAY:
            output = decayBase + output * decayCoef;
            if (output <= sustainLevel) {
                output = sustainLevel;
                state = State::SUSTAIN;
            }
            break;
        case State::SUSTAIN:
            output = sustainLevel;
            break;
        case State::RELEASE:
            output = releaseBase + output * releaseCoef;
            if (output <= 0.0) {
                output = 0.0;
                state = State::IDLE;
            }
            break;
    }

    return output;
}