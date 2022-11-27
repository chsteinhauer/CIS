
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"
#include "ButterworthBandpass.h"
#include "Carrier.h"

using Block = juce::dsp::AudioBlock<float>;

class ReconstructionExample {
public:
    ReconstructionExample();
    ~ReconstructionExample();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    void reset();

private:
    struct Synthesis {
        Synthesis();
        ~Synthesis();

        void prepare(const juce::dsp::ProcessSpec& spec);
        void process(const juce::dsp::ProcessContextReplacing<float>& context);
        void reset();

        Sine sine;
        Noise noise;
        juce::dsp::Gain<float> gain;
    };

    ButterworthBandpass butterworth;
    Synthesis synth;

};


