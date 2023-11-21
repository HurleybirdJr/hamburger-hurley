#pragma once

#include "../EnvelopeFollower.h"

#include <JuceHeader.h>

class MBComp
{
public:
    MBComp(juce::AudioProcessorValueTreeState &state) : compressor1(CompressionType::COMPRESSOR),
                                                        compressor2(CompressionType::COMPRESSOR),
                                                        compressor3(CompressionType::COMPRESSOR),
                                                        threshold(state, "compThreshold"),
                                                        ratio(state, "compRatio"),
                                                        attack(state, "compAttack"),
                                                        release(state, "compRelease"),
                                                        makeup(state, "compOut") {}

    ~MBComp() {}

    void processBlock(dsp::AudioBlock<float> &block)
    {
        attack.update();
        release.update();
        makeup.update();
        ratio.update();
        threshold.update();

        for (int sample = 0; sample < block.getNumSamples(); sample++)
        {
            float leftSample = block.getSample(0, sample);
            float rightSample = block.getSample(1, sample);

            float lowResultL = lowCrossOver.processSample(0, leftSample);
            float highResultL = highCrossOver.processSample(0, leftSample);
            float midResultL = leftSample - lowResultL - highResultL;

            float lowResultR = lowCrossOver.processSample(1, rightSample);
            float highResultR = highCrossOver.processSample(1, rightSample);
            float midResultR = rightSample - lowResultR - highResultR;

            lowBuffer.setSample(0, sample, lowResultL);
            midBuffer.setSample(0, sample, midResultL);
            highBuffer.setSample(0, sample, highResultL);
            lowBuffer.setSample(1, sample, lowResultR);
            midBuffer.setSample(1, sample, midResultR);
            highBuffer.setSample(1, sample, highResultR);
        }

        float atk = attack.getRaw();
        float rel = release.getRaw();
        float mkp = makeup.getRaw();
        float rat = ratio.getRaw();
        float thr = threshold.getRaw();

        // float atk, float rel, float mkp, float ratioLow, float ratioUp, float thresholdLow, float thresholdUp, float kneeW, float mkpDB)
        compressor1.updateUpDown(atk, rel, mkp, rat, rat, thr, thr + 2.0f, 0.1f, 0.f);
        compressor2.updateUpDown(atk, rel, mkp, rat, rat, thr, thr + 2.0f, 0.1f, 0.f);
        compressor3.updateUpDown(atk, rel, mkp, rat, rat, thr, thr + 2.0f, 0.1f, 0.f);
        
        compressor1.processBlock(lowBlock);
        compressor2.processBlock(midBlock);
        compressor3.processBlock(highBlock);

        for (int sample = 0; sample < block.getNumSamples(); sample++) {
            block.setSample(0, sample, 
                lowBuffer.getSample(0, sample) + 
                midBuffer.getSample(0, sample) + 
                highBuffer.getSample(0, sample));
            block.setSample(1, sample, 
                lowBuffer.getSample(1, sample) + 
                midBuffer.getSample(1, sample) + 
                highBuffer.getSample(1, sample));
        }
    }

    void prepare(dsp::ProcessSpec &spec)
    {
        compressor1.prepare(spec);
        compressor2.prepare(spec);
        compressor3.prepare(spec);

        lowCrossOver.setCutoffFrequency(500.0f);
        highCrossOver.setCutoffFrequency(2500.0f);
        lowCrossOver.setType(dsp::LinkwitzRileyFilterType::lowpass);
        highCrossOver.setType(dsp::LinkwitzRileyFilterType::highpass);
        lowCrossOver.prepare(spec);
        highCrossOver.prepare(spec);

        lowBuffer.setSize(2, spec.maximumBlockSize);
        midBuffer.setSize(2, spec.maximumBlockSize);
        highBuffer.setSize(2, spec.maximumBlockSize);

        lowBlock = dsp::AudioBlock<float>(lowBuffer);
        midBlock = dsp::AudioBlock<float>(midBuffer);
        highBlock = dsp::AudioBlock<float>(highBuffer);
    }

private:
    double sampleRate;

    SmoothParam threshold;
    SmoothParam ratio;
    SmoothParam attack;
    SmoothParam release;
    SmoothParam makeup;

    AudioBuffer<float> lowBuffer;
    AudioBuffer<float> midBuffer;
    AudioBuffer<float> highBuffer;
    dsp::AudioBlock<float> lowBlock;
    dsp::AudioBlock<float> midBlock;
    dsp::AudioBlock<float> highBlock;

    // mid-high 2.5khz
    // low - mid 500hz

    dsp::LinkwitzRileyFilter<float> lowCrossOver;
    dsp::LinkwitzRileyFilter<float> highCrossOver;

    Compressor compressor1;
    Compressor compressor2;
    Compressor compressor3;
};