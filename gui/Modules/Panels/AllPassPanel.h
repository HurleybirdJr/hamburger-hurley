#pragma once

#include <JuceHeader.h>

#include "../Panel.h"
#include "PanelNames.h"
#include "../../LookAndFeel/Palette.h" 

class AllPassPanel : public Panel
{
public:
    AllPassPanel(AudioPluginAudioProcessor &p) : Panel(p, "ALLPASS"),
        amount(p, "AMOUNT", "allPassAmount"),
        freq(p, "FREQ", "allPassFreq", ParamUnits::hz),
        q(p, "Q", "allPassQ")
    {

        addAndMakeVisible(amount);
        addAndMakeVisible(freq);
        addAndMakeVisible(q);
    }

    // void paint(juce::Graphics &g) override
    // {
    //     // g.fillAll(juce::Colours::green);
    // }

    void resized() override {
        // three, in a row
        auto bounds = getLocalBounds();
        amount.setBounds(bounds.removeFromLeft(bounds.getWidth() / 3));
        freq.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
        q.setBounds(bounds);
    }

    ParamKnob amount;
    ParamKnob freq;
    ParamKnob q;
};