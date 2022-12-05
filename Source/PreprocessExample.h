
#pragma once

#include <JuceHeader.h>

class PreprocessExample {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing< float >& context);
    void reset();
     
private:
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,juce::dsp::IIR::Coefficients<float>> iir;
    juce::dsp::Compressor<float> compressor;
    juce::dsp::NoiseGate<float> expander;
};

