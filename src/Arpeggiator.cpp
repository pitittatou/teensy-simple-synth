#include "Arpeggiator.h"

#include "Audio.h"

Arpeggiator::Arpeggiator(int SR, Synth& synth, float noteDuration, float noteInterval, Mode mode) : samplingRate(SR),
                                                                                                    synth(synth),
                                                                                                    sampleCounter(0) {
    setNoteDuration(noteDuration);
    setNoteInterval(noteInterval);
    setMode(mode);
    currentNote = chord.end();
}

void Arpeggiator::setNoteDuration(float duration) {
    noteDurationSamples = samplingRate * duration;
}

void Arpeggiator::setNoteInterval(float interval) {
    noteIntervalSamples = samplingRate * interval;
}

void Arpeggiator::resetChord() {
    if (mode == Mode::UP_AND_DOWN) {
        ascending = true;
    } else if (mode == Mode::DOWN_AND_UP) {
        ascending = false;
    }
}

void Arpeggiator::setMode(Mode _mode) {
    mode = _mode;
    resetChord();
}

void Arpeggiator::startNote(float f, int v) {
    chord[f] = v;
}

void Arpeggiator::endNote(float f) {
    auto toDelete = chord.find(f);
    if (toDelete == currentNote) {
        currentNote = getNextNote();
    }

    chord.erase(toDelete);

    if (chord.empty()) {
        currentNote = chord.end();
        resetChord();
    }
}

std::map<float, int>::iterator Arpeggiator::getNextNote() {
    if (chord.empty()) {
        return chord.end();
    }

    if (chord.size() == 1) {
        return chord.begin();
    }

    std::map<float, int>::iterator newNote;
    switch (mode) {
        case Mode::UP:
            if (currentNote == chord.end()) {
                newNote = chord.begin();
            } else if (std::next(currentNote) == chord.end()) {
                newNote = chord.begin();
            } else {
                newNote = std::next(currentNote);
            }
            break;

        case Mode::DOWN:
            if (currentNote == chord.end()) {
                newNote = std::prev(chord.end());
            } else if (currentNote == chord.begin()) {
                newNote = std::prev(chord.end());
            } else {
                newNote = std::prev(currentNote);
            }
            break;

        case Mode::UP_AND_DOWN:
        case Mode::DOWN_AND_UP:
            if (currentNote == chord.end()) {
                if (ascending) {
                    newNote = chord.begin();
                } else {
                    newNote = std::prev(chord.end());
                }
            } else if (ascending) {
                newNote = std::next(currentNote);

                if (newNote == chord.end()) {
                    ascending = false;
                    newNote = std::prev(currentNote);
                }
            } else if (currentNote == chord.begin()) {
                ascending = true;
                newNote = std::next(currentNote);
            } else {
                newNote = std::prev(currentNote);
            }
            break;
    }

    return newNote;
}

void Arpeggiator::reset() {
    for (auto& note : chord) {
        endNote(note.first);
    }
}

void Arpeggiator::tick() {
    // Serial.println(sampleCounter);
    auto it = activeNotes.begin();
    while (it != activeNotes.end()) {
        it->elapsed++;
        if (it->elapsed > noteDurationSamples) {
            synth.endNote(it->frequency);
            it = activeNotes.erase(it);
        } else {
            it++;
        }
    }

    if (chord.empty()) {
        sampleCounter = 0;
    } else if (sampleCounter <= 0) {
        currentNote = getNextNote();
        synth.startNote(currentNote->first, currentNote->second);
        activeNotes.push_back(Note{currentNote->first, 0});
        sampleCounter = noteDurationSamples;
    } else {
        sampleCounter--;
    }
}
