#include "Triangle.h"

#include <cmath>

Triangle::Triangle(int SR) : Waveform(SR),
                             increment(0.0),
                             triangle(0.5) {}

void Triangle::setFrequency(float f) {
    increment = f / samplingRate;
}

float Triangle::tick() {
    float currentSample = 2 * std::fabs(triangle * 2.0 - 1.0) - 1;

    triangle += increment;
    triangle = triangle - std::floor(triangle);

    return currentSample;
}
