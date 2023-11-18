#pragma once

#include <JuceHeader.h>
#include <cmath>

class Curves {
public:
    static float computeCompressorGain(float input_db, float threshold, float ratio, float kneeWidth);
    static float computeExpanderGain(float input_db, float ratio, float kneeWidth);
    static float computeUpwardsDownwardsGain(float x, float upperThres, float lowerThres, float upperR, float lowerR, float knee);
};

float Curves::computeCompressorGain(float input_db, float threshold, float ratio, float kneeWidth)
{
    // this is the soft-clip function from the compressor
    // https://www.desmos.com/calculator/f8zazgtwpe

    // input is supposed to be in DB
    // output is supposed to be in gain

    float output = -100.0;

    if (2 * (input_db - threshold) < -kneeWidth)
    {
        output = input_db;
    }
    else if (2 * (input_db - threshold) > kneeWidth)
    {
        output = threshold + (input_db - threshold) / ratio;
    }
    else
    {
        output = input_db + ((1 / ratio - 1) * pow((input_db - threshold + kneeWidth / 2), 2) / (2 * kneeWidth));
    }

    float gainReduction = 0.f;

    gainReduction = output - input_db;

    return juce::Decibels::decibelsToGain(gainReduction);
}

float Curves::computeExpanderGain(float input_db, float ratio, float kneeWidth)
{
    float output = -100.0;
    float thres = 0.f;

    if (2 * (input_db - thres) > kneeWidth) {
        // below knee
        output = input_db;
    }
    else if (2 * (input_db - thres) <= -kneeWidth) {
        output = thres + (input_db - thres) * ratio;
    } else {
        output = input_db + ((1 / ratio) * pow((input_db - thres + kneeWidth / 2), 2) / (2 * kneeWidth));
    }

    float gainReduction = 0.f;
    gainReduction = output - input_db;
    return juce::Decibels::decibelsToGain(gainReduction);
}

float Curves::computeUpwardsDownwardsGain(float x, float upperThres, float lowerThres, float upperR, float lowerR, float knee) {
    float output = -100.0;

    float a = upperThres - 2*knee;
    if (lowerThres >= upperThres - 2*knee) {
        lowerThres = a;
    }

    if (2 * (x - upperThres) > knee) {
        // line above knee for top part
        output = upperThres + (x - upperThres) / upperR;
    } else if (2 * (abs(x - lowerThres)) > knee) {
        // top knee
        output = x + ((1 / upperR - 1) * pow(x - upperThres + knee / 2, 2)) / (2 * knee);
    } else if (2 * (x < lowerThres) - knee) {
        // bottom knee
        output = lowerThres + (x - lowerThres) / lowerR;
    } else if (2 * (x > upperThres) + knee) {
        // above knee for bottom part
        output = x + ((1 / lowerR - 1) * pow(x - lowerThres + knee / 2, 2)) / (2 * knee);
    } else {
        // } else if (knee + 2 * lowerThres < 2 * x && 2 * x < 2 * upperThres - knee) {
        // in between the two thresholds
        output = x;
    }

    float gainReduction = 0.f;
    gainReduction = output - x;
    return juce::Decibels::decibelsToGain(gainReduction);
}