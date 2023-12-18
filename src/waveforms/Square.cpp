#include "Square.h"

#include <cmath>

Square::Square(int SR) : Waveform(SR) {}

void Square::setFrequency(float f) {
    period = samplingRate / f;
    halfPeriod = period / 2;
}

float Square::tick() {
    counter ++;
    counter = counter % period;
    return (counter <= halfPeriod) ? 1 : -1;
}
