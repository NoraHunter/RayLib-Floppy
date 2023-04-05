#pragma once
#include <cmath>

static float exponent = 1.0f;                 // Audio exponentiation value
static float averageVolume[400] = { 0.0f };

void ProcessAudio(void* buffer, unsigned int frames)
{
    float* samples = (float*)buffer;   // Samples internally stored as <float>s
    float average = 0.0f;               // Temporary average volume

    for (unsigned int frame = 0; frame < frames; frame++)
    {
        float* left = &samples[frame * 2 + 0], * right = &samples[frame * 2 + 1];

        *left = powf(fabsf(*left), exponent) * ((*left < 0.0f) ? -1.0f : 1.0f);
        *right = powf(fabsf(*right), exponent) * ((*right < 0.0f) ? -1.0f : 1.0f);

        average += fabsf(*left) / frames;   // accumulating average volume
        average += fabsf(*right) / frames;
    }

    // Moving history to the left
    for (int i = 0; i < 399; i++) averageVolume[i] = averageVolume[i + 1];

    averageVolume[399] = average;         // Adding last average value
}

void updateAudioDevice() {
    if (exponent <= 0.5f) exponent = 0.5f;
    if (exponent >= 3.0f) exponent = 3.0f;
}