
#pragma once

#include <JuceHeader.h>
#include "SimulationEngine.h"

class AnalyseExample {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing< float >& context);
    void reset();

private:
    static const int N = 4;
    std::array<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>, N> filters;

    float frequencies[N] = {400,1000,2000,3000};
    float filterQ[N] = {0.2f,0.4f,0.5f,0.6f};
};