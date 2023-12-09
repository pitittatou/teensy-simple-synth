#include "Voice.h"

#define ATTACK_RATE 0.2
#define DECAY_RATE 0.3
#define SUSTAIN_LEVEL 0.8
#define RELEASE_RATE 2.0
#define TARGET_RATIO_A 0.3
#define TARGET_RATIO_DR 0.0001

Voice::Voice(int SR, Synth& synth) : triangle(SR),
                                     envelope(SR, ATTACK_RATE, DECAY_RATE, SUSTAIN_LEVEL, RELEASE_RATE, TARGET_RATIO_A, TARGET_RATIO_DR),
                                     gain(0.0),
                                     frequency(0.0),
                                     samplingRate(SR),
                                     age(-1),
                                     available(true),
                                     released(true),
                                     synth(synth) {}

void Voice::setFrequency(float f) {
    triangle.setFrequency(f);
    frequency = f;
}

float Voice::getFrequency() {
    return frequency;
}

void Voice::startNote(float f, int v, int a) {
    setFrequency(f);
    envelope.startNote();
    age = a;
    gain = v / 127.0;
    available = false;
    released = false;
}

void Voice::endNote() {
    envelope.endNote();
    released = true;
}

void Voice::markAsAvailable() {
    int currAge = age;
    age = -1;
    synth.freeVoice(currAge);
    setFrequency(0);
    gain = 0;
    available = true;
}

bool Voice::isAvailable() {
    return available;
}

bool Voice::isReleased() {
    return released;
}

int Voice::getAge() {
    return age;
}

void Voice::setAge(int a) {
    age = a;
}

float Voice::tick() {
    float val = triangle.tick() * gain * envelope.tick();

    if (!available && envelope.isIdle()) {
        markAsAvailable();
    }

    return val;
}
