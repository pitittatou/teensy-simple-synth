#include <Audio.h>

#include "Synth.h"

Synth synth;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(synth, 0, out, 0);
AudioConnection patchCord1(synth, 0, out, 1);

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
