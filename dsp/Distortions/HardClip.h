#pragma once

#include <JuceHeader.h>
#include "../SmoothParam.h"

//==============================================================================
/*
 */
class HardClip
{
public:
    HardClip(juce::AudioProcessorValueTreeState& treeState);
    ~HardClip();

    void processBlock(dsp::AudioBlock<float>& block);
    void prepareToPlay(double sampleRate, int samplesPerBlock);
private:
    SmoothParam gain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HardClip)
};