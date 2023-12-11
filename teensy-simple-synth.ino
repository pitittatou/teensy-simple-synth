#include <Audio.h>

#include <memory>
#include <vector>

#include "src/Synth.h"
#include "src/Voice.h"
#include "src/envelopes/ADSREnvelope.h"
#include "src/waveforms/Triangle.h"

Synth synth;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(synth, 0, out, 0);
AudioConnection patchCord1(synth, 0, out, 1);

std::unique_ptr<std::vector<Voice>> createVoices() {
    std::unique_ptr<std::vector<Voice>> voices;

    for (auto i = 0; i < VOICE_NB; i++) {
        if (true) {
            auto waveform = std::make_unique<Triangle>(AUDIO_SAMPLE_RATE_EXACT);
            auto envelope = std::make_unique<ADSREnvelope>(AUDIO_SAMPLE_RATE_EXACT, 0.2, 0.3, 0.8, 2.0, 0.3, 0.0001);
            voices->push_back(Voice(AUDIO_SAMPLE_RATE_EXACT, std::move(waveform), std::move(envelope), synth));
        }
    }
    return voices;
}

float midiToFreq(byte note) {
    return pow(2.0, (note - 69.0) / 12.0) * 440.0;
}

void onNoteOn(byte channel, byte note, byte velocity) {
    synth.startNote(midiToFreq(note), velocity);
}

void onNoteOff(byte channel, byte note, byte velocity) {
    synth.endNote(midiToFreq(note));
}

void setup() {
    synth.setVoices(createVoices());
    Serial.begin(9600);
    AudioMemory(2);
    audioShield.enable();
    audioShield.volume(0.8);

    usbMIDI.setHandleNoteOn(onNoteOn);
    usbMIDI.setHandleNoteOff(onNoteOff);
}

void loop() {
    usbMIDI.read();
}
