
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"
#include "ButterworthBandpass.h"
#include "EnvelopeExtractor.h"

using Duplicator = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

class AnalyseExample {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing< float >& context);
    void reset();

private:
    EnvelopeExtractor extractor;
    ButterworthBandpass bandpass;
};

