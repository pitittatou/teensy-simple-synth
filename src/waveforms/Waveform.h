#ifndef waveform_h_
#define waveform_h_

class Waveform {
   public:
    Waveform(int SR);

    virtual void setFrequency(float f) = 0;
    virtual float tick() = 0;

   protected:
    int samplingRate;
};

#endif /* waveform_h_ */
