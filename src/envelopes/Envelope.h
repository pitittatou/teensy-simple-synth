#ifndef envelope_h_
#define envelope_h_

class Envelope {
   public:
    Envelope(int SR);

    virtual float tick() = 0;
    virtual void startNote() = 0;
    virtual void endNote() = 0;
    virtual bool isIdle() = 0;

   protected:
    int sampleRate;
};

#endif /* envelope_h_ */
