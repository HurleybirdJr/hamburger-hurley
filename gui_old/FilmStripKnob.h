
#pragma once

#include <JuceHeader.h>

#include "../PluginProcessor.h"
#include "./LookAndFeel/SaturationLookAndFeel.h"
#include "./LookAndFeel/LabelLookAndFeel.h"

// #include "ParameterKnob.h"

class FilmStripKnob : public juce::Slider, private juce::Slider::Listener
{
public:
    FilmStripKnob(AudioPluginAudioProcessor &p, juce::String knobIdParam, juce::String nameParam, float minRange, float maxRange, juce::Image image, int knobSize)
        : processorRef(p), pluginLookAndFeel(knobSize, image, juce::ImageCache::getFromMemory(BinaryData::knobShadow2_png, BinaryData::knobShadow2_pngSize))
    {
        knobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, knobIdParam, knob);

        knob.setLookAndFeel(&pluginLookAndFeel);
        knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setRange(minRange, maxRange, 0.00001f);
        knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
        knob.setTextValueSuffix(" " + nameParam);
        
        auto font = Font(Typeface::createSystemTypefaceFor(BinaryData::QuicksandBold_ttf, BinaryData::QuicksandBold_ttfSize));
        addAndMakeVisible(&knob);

        // addListener(this);
        knobLabel.setText(nameParam, juce::NotificationType::dontSendNotification);
        knobLabel.toFront(false);
        knobLabel.setJustificationType(juce::Justification::horizontallyCentred);
        knobLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromRGB(84, 84, 84));
        font = Font(Typeface::createSystemTypefaceFor(BinaryData::QuicksandSemiBold_ttf, BinaryData::QuicksandSemiBold_ttfSize));
        font.setSizeAndStyle(knobSize * 0.15 + 9, "SemiBold", 1, 0);
        knobLabel.setFont(font);
        addAndMakeVisible(&knobLabel);

        this->knobSize = knobSize;
        knobName = nameParam;
    };

    ~FilmStripKnob()
    {
        knobAttachment = nullptr; // if this is not here, plugin will crash
    }

    void paint(juce::Graphics &g) override
    {
        // paint bg to see the area occupied by this element
        g.fillAll(juce::Colours::red);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        auto knobBounds = bounds.removeFromTop(getKnobSize() - 3);

        knobBounds.reduce(8, 8);

        // auto rangeLabels = bounds.removeFromTop(15);

        knob.setBounds(knobBounds);
        // knobMinRangeLabel.setBounds(rangeLabels.removeFromLeft(bounds.getWidth() * 0.45));
        // knobMaxRangeLabel.setBounds(rangeLabels.removeFromRight(bounds.getWidth() * 0.45)); // by now the bounds are modified far enough to keep this as it is
        knobLabel.setBounds( bounds.removeFromTop(16));
    }

    int getKnobSize()
    {
        return knobSize + 10;
    }

    int getKnobHeight()
    {
        return knobSize + 30;
    }

    void sliderValueChanged(juce::Slider *slider)
    {
        juce::ignoreUnused(slider);
    };

private:
    AudioPluginAudioProcessor &processorRef;
    SaturationLookAndFeel pluginLookAndFeel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> knobAttachment = nullptr;

    int knobSize;
    juce::String knobName;
    juce::Slider knob;

    juce::Label knobLabel;
    juce::Label knobMinRangeLabel;
    juce::Label knobMaxRangeLabel;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmStripKnob)
};
