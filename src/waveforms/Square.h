#ifndef square_h_
#define square_h_

#include "Waveform.h"

class Square : public Waveform {
   public:
    Square(int SR);

    void setFrequency(float f) override;
    float tick() override;

   private:
    int period;
    int halfPeriod;
    int counter;
};

#endif /* square_h_ */
