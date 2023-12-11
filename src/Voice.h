#ifndef voice_h_
#define voice_h_

#include <memory>

#include "Synth.h"
#include "envelopes/Envelope.h"
#include "waveforms/Waveform.h"

class Synth;

class Voice {
   public:
    Voice(int SR, std::unique_ptr<Waveform>&& waveform, std::unique_ptr<Envelope>&& envelope, Synth& synth);

    void startNote(float f, int v, int a);
    void endNote();
    float tick();
    bool isAvailable();
    bool isReleased();
    float getFrequency();
    int getAge();
    void setAge(int a);

   private:
    std::unique_ptr<Waveform> waveform;
    std::unique_ptr<Envelope> envelope;
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
