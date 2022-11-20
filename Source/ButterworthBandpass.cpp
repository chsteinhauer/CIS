/*
  ==============================================================================

	ButterworthBandpass.cpp
	Created: 13 Nov 2022 12:11:38pm
	Author:  Sonderbo

  ==============================================================================
*/

#include "ButterworthBandpass.h"

ButterworthBandpass::ButterworthBandpass(int numChannels, int samplingFrequency)
{
	remakeFilters(numChannels, samplingFrequency);
}

void ButterworthBandpass::remakeFilters(int numChannels, int samplingFrequency)
{
	clearFilters();

	float lowFreq = greenwood(0);
	float highFreq;
	for (int i = 1; i <= numChannels; i++)
	{
		highFreq = greenwood((float)i / (numChannels));

		juce::OwnedArray<juce::dsp::IIR::Filter<float>>* lowpass;
		juce::OwnedArray<juce::dsp::IIR::Filter<float>>* highpass;

		auto lowpassCoeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(lowFreq, samplingFrequency, 6);
		for (auto coeff : lowpassCoeffs)
			lowpass->add(new juce::dsp::IIR::Filter<float>(coeff));
		lowPassArray.add(lowpass);

		auto highpassCoeffs = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(highFreq, samplingFrequency, 6);
		for (auto coeff : lowpassCoeffs)
			highpass->add(new juce::dsp::IIR::Filter<float>(coeff));
		highPassArray.add(highpass);
		lowFreq = highFreq;
	}
}

void ButterworthBandpass::clearFilters()
{
	for (int i = 0; i < lowPassArray.size(); i++)
	{
		lowPassArray[i]->clear();
	}
	lowPassArray.clear();
	for (int i = 0; i < highPassArray.size(); i++)
	{
		highPassArray[i]->clear();
	}
	highPassArray.clear();
}

void ButterworthBandpass::process(juce::dsp::AudioBlock<float> block)
{
	for (auto filters : lowPassArray)
		for (int i = 0; i < filters->size(); i++)
		{
			juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));
			(*filters)[0]->process(context);
		}

	for (auto filters : highPassArray)
		for (int i = 0; i < filters->size(); i++)
		{
			juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));
			(*filters)[0]->process(context);
		}
}

float ButterworthBandpass::greenwood(float x)
{
	//Constants for greenwood function applied to the human cochlear
	const float A = 165.4f;
	const float a = 2.1f;
	const float K = 0.88f;
	
	const float min = 0.0f;
	const float max = 1.0f;
	const float newMin = 0.180318f;
	const float newMax = 0.689763f;

	x = map(x, min, max, newMin, newMax);

	if (x > 1 || x < 0)
	{
		return -1;
	}
	return A * (pow(10, (a * x)) - K);
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}