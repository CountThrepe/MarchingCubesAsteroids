#ifndef TEXTUREGENERATOR_HPP
#define TEXTUREGENERATOR_HPP

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#include <GL/glu.h>
#include <GL/glext.h>
#include <math.h>

#include "OpenSimplexNoise.hpp"

struct layer{
    float scale;
    float strength;
    float red, green, blue;
    OpenSimplexNoise::Noise* noise;
};

class TextureGenerator {
public:
    TextureGenerator(int, int);
    ~TextureGenerator();
    unsigned int getTexture();
    void scramble();
    int getNumLayers();
    void setNumLayers(int);
    void setLayerScale(int, float);
    void setLayerStrength(int, float);
    void setLayerColor(int, float, float, float);

private:
    int nLayers, samplingRate;
    layer* layers;
    float strSum;

    int threeToOne(int, int, int);
};

#endif