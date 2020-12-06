#include "TextureGenerator.hpp"

TextureGenerator::TextureGenerator(int numLayers, int sampling) {
    this->samplingRate = sampling;

    this->nLayers = 0;
    this->layers = NULL;
    this->setNumLayers(numLayers);

    this->strSum = 0;
    for(int i = 0; i < numLayers; i++) {
        float strength = 1 - (i / (float) numLayers);
        strength = pow(strength, 0.5);
        layers[i].strength = strength;
        this->strSum += strength;

        float scale = (i+2) * (i+2) / 2.0;
        layers[i].scale = scale;


        layers[i].red = 1;
        layers[i].green = 1;
        layers[i].blue = 1;
    }
}

TextureGenerator::~TextureGenerator() {
    for(int i = 0; i < this->nLayers; i++) {
        delete layers[i].noise;
    }
    delete[] layers;
}

int TextureGenerator::threeToOne(int x, int y, int z) {
    int index = 3 * z;
    index += 3 * samplingRate * y;
    index += 3 * samplingRate * samplingRate * x;
    return index;
}

unsigned int TextureGenerator::getTexture() {
    int size = 3 * samplingRate * samplingRate * samplingRate;
    unsigned char* texture = new unsigned char[size];
    for(int x = 0; x < samplingRate; x++) {
        for(int y = 0; y < samplingRate; y++) {
            for(int z = 0; z < samplingRate; z++) {
                float red = 0, green = 0, blue = 0;
                for(int i = 0; i < nLayers; i++) {
                    float val = layers[i].noise->eval(x * layers[i].scale / samplingRate,
                                                      y * layers[i].scale / samplingRate,
                                                      z * layers[i].scale / samplingRate); // Number between -1 & 1
                    val *= layers[i].strength;

                    if(i == 0) {
                        val = (val + 1) / 2;
                        red = (val * layers[i].red * 2) - 1;
                        green = (val * layers[i].green * 2) - 1;
                        blue = (val * layers[i].blue * 2) - 1;
                    } else {
                        red += val * layers[i].red;
                        green += val * layers[i].green;
                        blue += val * layers[i].blue;
                    }
                }

                // Normalize colors;
                red /= strSum;
                red = (red + 1) / 2;
                green /= strSum;
                green = (green + 1) / 2;
                blue /= strSum;
                blue = (blue + 1) / 2;

                int index = threeToOne(x, y, z);
                texture[index] = red * 255;
                texture[index + 1] = green * 255;
                texture[index + 2] = blue * 255;
            }
        }
    }

    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexImage3D(GL_TEXTURE_3D, 0, 3, samplingRate, samplingRate, samplingRate, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    if (glGetError()) {
        printf("AHHHHHHHHHH!");
        textureID = -1;
    }
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete[] texture;
    return textureID;
}

void TextureGenerator::scramble() {
    for(int i = 0; i < this->nLayers; i++) {
        delete layers[i].noise;
        layers[i].noise = new OpenSimplexNoise::Noise();
    }
}

int TextureGenerator::getNumLayers() {
    return this->nLayers;
}

void TextureGenerator::setNumLayers(int n) {
    for(int i = 0; i < this->nLayers; i++) {
        delete layers[i].noise;
    }
    delete[] layers;

    this->nLayers = n;
    layers = new layer[n];
    for(int i = 0; i < n; i++) {
        layers[i].noise = new OpenSimplexNoise::Noise();
    }
}

void TextureGenerator::setLayerScale(int i, float s) {
    if(i >= this->nLayers || i < 0 || s < 0) return;
    layers[i].scale = s;
}

void TextureGenerator::setLayerStrength(int i, float s) {
    if(i >= this->nLayers || i < 0 || s > 1 || s < 0) return;
    strSum -= layers[i].strength;
    strSum += s;
    layers[i].strength = s;
}

void TextureGenerator::setLayerColor(int i, float r, float g, float b) {
    if(i >= this->nLayers || i < 0 || r > 1 || r < 0 || g > 1 || g < 0 || b > 1 || b < 0) return;
    layers[i].red = r;
    layers[i].green = g;
    layers[i].blue = b;
}