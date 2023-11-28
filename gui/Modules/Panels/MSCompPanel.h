#pragma once

 

#include "../Panel.h"
#include "PanelNames.h"

class MSCompPanel : public Panel
{
public:
    MSCompPanel(AudioPluginAudioProcessor &p) : Panel(p, "MIDSIDE"),
                                                   threshold(p, "THRES", "compThreshold", ParamUnits::db),
                                                   ratio(p, "RATIO", "compRatio"),
                                                   tilt(p, "TILT", "compBandTilt", ParamUnits::db),
                                                   attack(p, "SPEED", "compSpeed", ParamUnits::ms),
                                                   makeup(p, "GAIN", "compOut", ParamUnits::db)
    {
        addAndMakeVisible(threshold);
        addAndMakeVisible(ratio);
        addAndMakeVisible(tilt);
        addAndMakeVisible(attack);
        addAndMakeVisible(makeup);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();

        using fr = Grid::Fr;
        using Track = Grid::TrackInfo;

        grid.templateRows = {Track(fr(1)), Track(fr(1))};
        grid.templateColumns = {Track(fr(1)), Track(fr(1)), Track(fr(1))};

        grid.items = {
            GridItem(threshold).withArea(1, 1),
            GridItem(tilt).withArea(1, 2),
            GridItem(ratio).withArea(1, 3),
            GridItem(attack).withArea(2, 1),
            GridItem(makeup).withArea(2, 3)};

        grid.performLayout(bounds);
    }

private:
    Grid grid;

    ParamKnob threshold;
    ParamKnob ratio;
    ParamKnob tilt;
    ParamKnob attack;
    ParamKnob makeup;
};