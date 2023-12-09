#include "Synth.h"

#define AUDIO_OUTPUTS 1

#define MULT_16 32767
#define VOICE_NB 16

Synth::Synth() : AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS]),
                 voices(VOICE_NB, Voice(AUDIO_SAMPLE_RATE_EXACT, *this)),
                 maxAge(-1) {}

void Synth::freeVoice(int age) {
    int voice_age;
    for (Voice& voice : voices) {
        voice_age = voice.getAge();
        if (voice_age > age) {
            voice.setAge(voice_age - 1);
        }
    }
    maxAge--;
}

void Synth::startNote(float f, int v) {
    for (int i = 0; i < voices.size(); i++) {
        if (voices[i].isAvailable()) {
            voices[i].startNote(f, v, ++maxAge);
            return;
        }
    }

    // If we get here, that means all voices are unavailable
    // We steal the oldest one
    for (Voice& voice : voices) {
        if (voice.getAge() == 0) {
            voice.startNote(f, v, maxAge);
        } else {
            voice.setAge(voice.getAge() - 1);
        }
    }
}

void Synth::endNote(float f) {
    for (Voice& voice : voices) {
        if (voice.getFrequency() == f && !voice.isReleased()) {
            voice.endNote();
        }
    }
}

void Synth::update(void) {
    audio_block_t* outBlock[AUDIO_OUTPUTS];
    for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
        outBlock[channel] = allocate();
        if (outBlock[channel]) {
            for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
                float currentSample = 0.0;
                for (Voice& voice : voices) {
                    currentSample += voice.tick();
                }
                currentSample /= VOICE_NB;
                currentSample = max(-1, min(1, currentSample));
                int16_t val = currentSample * MULT_16;
                outBlock[channel]->data[i] = val;
            }
            transmit(outBlock[channel], channel);
            release(outBlock[channel]);
        }
    }
}
