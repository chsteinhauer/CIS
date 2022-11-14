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

	float lowFreq = 20.0;
	float highFreq = greenwood(0);
	int i = 1;
	for (;;)
	{
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

		if (i + 1 < numChannels)
		{
			break;
		}

		lowFreq = highFreq;
		highFreq = greenwood((float)i / (numChannels - 1));
		i++;
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

	if (x > 1 || x < 0)
	{
		return -1;
	}
	return A * (pow(10, (a * x)) - K);
}