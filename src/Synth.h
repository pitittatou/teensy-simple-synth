#ifndef synth_h_
#define synth_h_

#include <memory>
#include <vector>

#include "Arduino.h"
#include "Audio.h"
#include "AudioStream.h"
#include "Voice.h"
#include "Arpeggiator.h"


#define VOICE_NB 16

class Voice;
class Arpeggiator;

class Synth : public AudioStream {
   public:
    Synth();

    std::unique_ptr<Arpeggiator> arpeggiator;

    virtual void update(void);
    void setVoices(std::unique_ptr<std::vector<Voice>>&& voices);
    void startNote(float f, int v);
    void endNote(float f);
    void freeVoice(int age);
    void reset();

   private:
    std::unique_ptr<std::vector<Voice>> voices;
    int maxAge;
};

#endif /* synth_h_ */
