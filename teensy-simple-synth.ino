#include <Audio.h>

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "src/Synth.h"
#include "src/Voice.h"
#include "src/envelopes/ADSREnvelope.h"
#include "src/waveforms/Triangle.h"

#define VOICE_SOUND_NB 1

Synth synth;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(synth, 0, out, 0);
AudioConnection patchCord1(synth, 0, out, 1);
bool arpeggiatorOn;

std::unique_ptr<std::vector<Voice>> createVoices(int voiceSound) {
    auto voices = std::make_unique<std::vector<Voice>>();

    for (auto i = 0; i < VOICE_NB; i++) {
        if (voiceSound == 0) {
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
    float f = midiToFreq(note);
    if (arpeggiatorOn) {
        synth.arpeggiator->startNote(f, velocity);
    } else {
        synth.startNote(f, velocity);
    }
}

void onNoteOff(byte channel, byte note, byte velocity) {
    float f = midiToFreq(note);
    if (arpeggiatorOn) {
        synth.arpeggiator->endNote(f);
    } else {
        synth.endNote(f);
    }
}

void onControlChange(byte channel, byte control, byte value) {
    if (control == 7) {
        audioShield.volume(value / 127.0);
    }
}

void processInput(String input) {
    if (input.startsWith("voice ")) {
        int voiceSound = input.substring(6).toInt();
        if (voiceSound < VOICE_SOUND_NB && voiceSound >= 0) {
            synth.setVoices(createVoices(voiceSound));
            Serial.print("Switched to voice sound ");
            Serial.println(voiceSound);
        }
    } else if (input.startsWith("arp ")) {
        String s = input.substring(4);
        if (s.startsWith("on")) {
            arpeggiatorOn = true;
            synth.reset();
            Serial.println("Arpeggiator on");
        } else if (s.startsWith("off")) {
            arpeggiatorOn = false;
            synth.arpeggiator->reset();
            Serial.println("Arpeggiator off");
        } else if (s.startsWith("mode ")) {
            String mode = s.substring(5);
            if (mode.startsWith("updown")) {
                synth.arpeggiator->setMode(Arpeggiator::Mode::UP_AND_DOWN);
                Serial.println("Arpeggiator mode set to UP_AND_DOWN");
            } else if (mode.startsWith("downup")) {
                synth.arpeggiator->setMode(Arpeggiator::Mode::DOWN_AND_UP);
                Serial.println("Arpeggiator mode set to DOWN_AND_UP");
            } else if (mode.startsWith("up")) {
                synth.arpeggiator->setMode(Arpeggiator::Mode::UP);
                Serial.println("Arpeggiator mode set to UP");
            } else if (mode.startsWith("down")) {
                synth.arpeggiator->setMode(Arpeggiator::Mode::DOWN);
                Serial.println("Arpeggiator mode set to DOWN");
            }
        } else if (s.startsWith("note ")) {
            float noteDuration = s.substring(5).toFloat();
            if (noteDuration != 0.0) {
                synth.arpeggiator->setNoteDuration(noteDuration);
                Serial.print("Arpeggiator note duration set to ");
                Serial.println(noteDuration);
            }
        } else if (s.startsWith("inter ")) {
            float noteInterval = s.substring(6).toFloat();
            if (noteInterval != 0.0) {
                synth.arpeggiator->setNoteInterval(noteInterval);
                Serial.print("Arpeggiator note interval set to ");
                Serial.println(noteInterval);
            }
        }
    }
}

void setup() {
    synth.setVoices(createVoices(0));
    arpeggiatorOn = false;

    Serial.begin(9600);
    Serial.setTimeout(0);

    AudioMemory(2);
    audioShield.enable();
    audioShield.volume(0.7);

    usbMIDI.setHandleNoteOn(onNoteOn);
    usbMIDI.setHandleNoteOff(onNoteOff);
    usbMIDI.setHandleControlChange(onControlChange);
}

void loop() {
    String receivedString = Serial.readStringUntil('\n');
    if (receivedString.length() > 0) {
        processInput(receivedString);
    }

    usbMIDI.read();
}
