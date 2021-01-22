#ifndef _WAVESHAPER
#define _WAVESHAPER

#include <string>

const int DIST_TYPES = 2;
const int WTABLE_SIZE = 1024;

class Waveshaper
{
public:
    enum WSMode
    {
        polyDist = 0,
        tanhDist
    };
    Waveshaper();
    float processWaveshape(float in);
    void setDistMode(std::string& mode);
    WSMode getDistMode(); 
    void setGain(float gain);

private:
    // Methods
    void initLookupTables();
    float computePolynomial(float in);
    float adjustRange(int in, int totalSamples, int newRange);

    // Attributes
    WSMode distMode = polyDist;
    float tables[DIST_TYPES][WTABLE_SIZE];
    float inGain = 1.0f;
};
#endif