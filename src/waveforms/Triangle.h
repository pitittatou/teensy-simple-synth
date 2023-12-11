#ifndef triangle_h_
#define triangle_h_

#include "Waveform.h"

class Triangle : public Waveform {
   public:
    Triangle(int SR);

    void setFrequency(float f) override;
    float tick() override;

   private:
    float increment;
    float triangle;
};

#endif /* triangle_h_ */
