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

    for (int i = 0; i < numChannels)
    {
        juce::OwnedArray<juce::dsp::IIR::Filter<float>>* lowpass;
        juce::OwnedArray<juce::dsp::IIR::Filter<float>>* highpass;

        auto lowpassCoeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(freq, samplingFrequency, 6);
        for (auto coeff : lowpassCoeffs)
            lowpass->add(new juce::dsp::IIR::Filter<float>(coeff));
        lowPassArray.add(lowpass);

        auto highpassCoeffs = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(freq, samplingFrequency, 6);
        for (auto coeff : lowpassCoeffs)
            highpass->add(new juce::dsp::IIR::Filter<float>(coeff));
        highPassArray.add(highpass);
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

}

int greenwood()
{

}