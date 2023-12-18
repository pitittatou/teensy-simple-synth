#ifndef arpeggiator_h_
#define arpeggiator_h_

#include <map>
#include <vector>

#include "Synth.h"

class Synth;

class Arpeggiator {
   public:
    enum class Mode {
        UP,
        DOWN,
        UP_AND_DOWN,
        DOWN_AND_UP
    };

    Arpeggiator(int SR, Synth& synth, float noteDuration, float noteInterval, Mode mode);

    void setNoteDuration(float duration);
    void setNoteInterval(float interval);
    void resetChord();
    void setMode(Mode _mode);
    void startNote(float f, int v);
    void endNote(float f);
    void reset();
    void tick();

   private:
    struct Note {
        float frequency;
        float elapsed;
    };

    int samplingRate;
    Synth& synth;
    float noteDurationSamples;
    float noteIntervalSamples;
    int sampleCounter;
    Mode mode;
    bool ascending;
    std::map<float, int> chord;
    std::map<float, int>::iterator currentNote;
    std::vector<Note> activeNotes;

    std::map<float, int>::iterator getNextNote();
};

#endif /* arpeggiator_h_ */
