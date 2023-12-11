#include "Voice.h"

Voice::Voice(int SR, std::unique_ptr<Waveform>&& waveform,
             std::unique_ptr<Envelope>&& envelope, Synth& synth) : waveform(std::move(waveform)),
                                                                   envelope(std::move(envelope)),
                                                                   gain(0.0),
                                                                   frequency(0.0),
                                                                   samplingRate(SR),
                                                                   age(-1),
                                                                   available(true),
                                                                   released(true),
                                                                   synth(synth) {}

void Voice::setFrequency(float f) {
    waveform->setFrequency(f);
    frequency = f;
}

float Voice::getFrequency() {
    return frequency;
}

void Voice::startNote(float f, int v, int a) {
    setFrequency(f);
    envelope->startNote();
    age = a;
    gain = v / 127.0;
    available = false;
    released = false;
}

void Voice::endNote() {
    envelope->endNote();
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
    float val = gain * waveform->tick() * envelope->tick();

    if (!available && envelope->isIdle()) {
        markAsAvailable();
    }

    return val;
}
