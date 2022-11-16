
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"

using Block = juce::dsp::AudioBlock<float>;

class ReconstructionExample {
public:
    ReconstructionExample();
    ~ReconstructionExample();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    void reset();

private:
    juce::dsp::Oscillator<float> osc;
    juce::Random random;
};


