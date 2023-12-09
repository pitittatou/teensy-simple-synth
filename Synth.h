#ifndef synth_h_
#define synth_h_

#include <vector>

#include "Arduino.h"
#include "Audio.h"
#include "AudioStream.h"
#include "Voice.h"

class Voice;

class Synth : public AudioStream {
   public:
    Synth();

    virtual void update(void);
    void startNote(float f, int v);
    void endNote(float f);
    void freeVoice(int age);

   private:
    std::vector<Voice> voices;
    int maxAge;
};

#endif /* synth_h_ */
