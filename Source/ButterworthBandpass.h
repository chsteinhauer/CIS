

#pragma once
#include <JuceHeader.h>

using FilterArray = juce::OwnedArray<juce::OwnedArray<juce::dsp::IIR::Filter<float>>>;
using FilterVector = std::vector<std::vector<juce::dsp::IIR::Filter<float>*>>;

#pragma once
class ButterworthBandpass
{
public:
    ButterworthBandpass();
    ~ButterworthBandpass();

    void remakeFilters(int numChannels, int samplingFrequency);
    void process(juce::dsp::AudioBlock<float> block);
    void clearFilters();

private:
    std::unique_ptr<FilterVector> lowPassArray;
    std::unique_ptr <FilterVector> highPassArray;

    float greenwood(float x);

    //std::unique_ptr<FilterVector> bandpasses;
};