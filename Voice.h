#ifndef voice_h_
#define voice_h_

#include "ADSREnvelope.h"
#include "Synth.h"
#include "TriangleWave.h"

class Synth;

class Voice {
   public:
    Voice(int SR, Synth& synth);

    void startNote(float f, int v, int a);
    void endNote();
    float tick();
    bool isAvailable();
    bool isReleased();
    float getFrequency();
    int getAge();
    void setAge(int a);

   private:
    TriangleWave triangle;
    ADSREnvelope envelope;
    float gain;
    float frequency;
    int samplingRate;
    int age;
    bool available;
    bool released;
    Synth& synth;

    void setFrequency(float f);
    void markAsAvailable();
};

#endif /* voice_h_ */
