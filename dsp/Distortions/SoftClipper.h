#pragma once

#include "WaveShapers.h"
#include "../SmoothParam.h"

#include "juce_dsp/juce_dsp.h"
#include "juce_audio_processors/juce_audio_processors.h"

#include <melatonin_perfetto/melatonin_perfetto.h>

class SoftClip
{
public:
    SoftClip(juce::AudioProcessorValueTreeState& treeState);

    ~SoftClip();

    void processBlock(juce::dsp::AudioBlock<float>& block);
    void prepare(juce::dsp::ProcessSpec& spec);
private:
    SmoothParam saturationKnob;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoftClip)
};