#include <iostream>
#include <math.h>
#include <string.h>
#include "waveshaper.h"

Waveshaper::Waveshaper()
{
    initLookupTables();
}

void Waveshaper::setGain(float gain)
{
    inGain = gain;
}

Waveshaper::WSMode Waveshaper::getDistMode()
{
    return distMode;
}

void Waveshaper::setDistMode(std::string& mode)
{
    if (mode == "poly")
    {
        distMode = polyDist;
    } else if (mode == "tanh")
    {
        distMode = tanhDist;
    } else
    {
        std::cout << "Unknown distortion type" << std::endl;
    }
}

float Waveshaper::computePolynomial(float in)
{
    float thres1 = 1.0f/3.0f, thres2 = 2.0f/3.0f, out = 0.0f;

    if (in > thres2)
    {
        out = 1.0f;
    } else if (in < -thres2)
    {
        out = -1.0f;
    } else if (in > thres1)
    {
        out = 1.0f - ((2.0f - 3.0f * in) * (2.0f - 3.0f * in) / 3.0f);
    } else if (in < -thres1)
    {
        out = -(1.0f - ((2.0f + 3.0f * in) * (2.0f + 3.0f * in) / 3.0f));
    } else
    {
        out = 2.0f * in;
    }
    return out;
}

float Waveshaper::adjustRange(int in, int totalSamples, int newRange)
{
    return ((2 * float(newRange) * float(in)) / float(totalSamples)) - float(newRange);
}

void Waveshaper::initLookupTables()
{
    float value, preGain = 4.0f;
    for (int s = 0; s < WTABLE_SIZE; ++s)
    {
        value = adjustRange(s, WTABLE_SIZE, 1);
        tables[WSMode::polyDist][s] = computePolynomial(value); 
        tables[WSMode::tanhDist][s] = float(tanh(double(value) * preGain));
    }
}

float Waveshaper::processWaveshape(float in)
{
    in += 1.f;
    in *= WTABLE_SIZE / 2;
    int inTrunc = int(in * inGain);
    float inDec = in - float(inTrunc);

    if (inTrunc >= WTABLE_SIZE - 1) // Allow one sample overhead for interpolation later
    {
        return 1.0f;
    }
    if (inTrunc < 1)
    {
        return -1.0f;
    }

    return (1 - inDec) * tables[distMode][inTrunc] + inDec * tables[distMode][(inTrunc + 1)];
}
