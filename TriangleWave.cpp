#include "TriangleWave.h"

#include <cmath>

TriangleWave::TriangleWave(int SR) : samplingRate(SR),
                                     increment(0.0),
                                     triangle(0.5) {}

void TriangleWave::setFrequency(float f) {
    increment = f / samplingRate;
}

float TriangleWave::tick() {
    float currentSample = 2 * std::fabs(triangle * 2.0 - 1.0) - 1;

    triangle += increment;
    triangle = triangle - std::floor(triangle);

    return currentSample;
}
