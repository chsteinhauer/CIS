
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"

using Duplicator = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

class AnalyseExample {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing< float >& context);
    void reset();

    void prepareFilters(const juce::dsp::ProcessSpec& spec, int channel)
    {
        auto value = (channel + 1) / spec.numChannels;
        auto Fc = State::GetInstance()->getParameter("Fc")->convertFrom0to1(value);

        auto bp = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, Fc);
    }

private:
    juce::dsp::ProcessorChain<Duplicator> filters;
};

