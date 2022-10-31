
#pragma once

#include <JuceHeader.h>
#include "SimulationEngine.h"

class ReconstructionExample {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    void reset();

};