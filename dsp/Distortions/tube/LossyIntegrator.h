#pragma once

#include "juce_core/juce_core.h"
#include "juce_dsp/juce_dsp.h"

class LossyIntegrator
{
public:
	LossyIntegrator() {}
	~LossyIntegrator() {}

	void prepare(juce::dsp::ProcessSpec& spec);
	
	juce::dsp::SIMDRegister<float> processAudioSample(juce::dsp::SIMDRegister<float> xn);
	float processAudioSample(float xn);

	void calculateFilterCoeffs();

protected:
	float sampleRate = 44100.0f;				///< current sample rate
	float T = 1.0f / sampleRate;	///<  1 / sample rate

	// random coefficients for coefficient updates
	const float fc = 4.0f;
	const float Q = 0.607f;
	const float R = 0.5f / Q;
	const float wd = juce::MathConstants<float>::twoPi * fc;

	juce::dsp::SIMDRegister<float> integrator_z[2];						///< state variables

	float alpha0 = 0.0f;		///< input scalar, correct delay-free loop
	float alpha = 0.0f;			///< alpha is (wcT/2)
	float rho = 0.0f;			///< p = 2R + g (feedback)

	float beta = 0.0f;			///< beta value, not used

	// --- for analog Nyquist matching
	float analogMatchSigma = 0.0f; ///< analog matching Sigma value (see book)
};