#ifndef adsr_envelope_h_
#define adsr_envelope_h_

class ADSREnvelope {
   public:
    ADSREnvelope(int SR, float attackRate, float decayRate, float sustainLevel, float releaseRate, float targetRatioA, float targetRatioDR);

    float tick();
    void startNote();
    void endNote();
    bool isIdle();

   private:
    enum class State {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    int sampleRate;
    State state;
    float output;
    float attackRate;
    float decayRate;
    float sustainLevel;
    float releaseRate;
    float attackCoef;
    float decayCoef;
    float releaseCoef;
    float targetRatioA;
    float targetRatioDR;
    float attackBase;
    float decayBase;
    float releaseBase;

    float calcCoef(float rate, float targetRatio);
    void setAttackRate(float rate);
    void setDecayRate(float rate);
    void setReleaseRate(float rate);
    void setSustainLevel(float level);
    void setTargetRatioA(float targetRatio);
    void setTargetRatioDR(float targetRatio);
};

#endif /* adsr_envelope_h_ */
