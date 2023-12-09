#ifndef triangle_wave_h_
#define triangle_wave_h_

class TriangleWave {
   public:
    TriangleWave(int SR);

    void setFrequency(float f);
    float tick();

   private:
    int samplingRate;
    float increment;
    float triangle;
};

#endif /* triangle_wave_h_ */
