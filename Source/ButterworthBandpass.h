/*
  ==============================================================================

    ButterworthBandpass.h
    Created: 13 Nov 2022 12:11:38pm
    Author:  Sonderbo

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once
class ButterworthBandpass
{
public:
    ButterworthBandpass(int numChannels, int samplingFrequency);

    void remakeFilters(int numChannels, int samplingFrequency);

    void process(juce::dsp::AudioBlock<float> block);
private:
    juce::OwnedArray<juce::OwnedArray<juce::dsp::IIR::Filter<float>>> lowPassArray;
    juce::OwnedArray<juce::OwnedArray<juce::dsp::IIR::Filter<float>>> highPassArray;

    void clearFilters();
    float greenwood(float x);
    float map(float x, float in_min, float in_max, float out_min, float out_max);
};