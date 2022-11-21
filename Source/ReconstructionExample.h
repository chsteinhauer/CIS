
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"
#include "ButterworthBandpass.h"

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

        juce::Random random;
        juce::dsp::Gain<float> gain;
        double sampleRate;

        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> iir;
    };

    ButterworthBandpass butterworth;
    Synthesis synth;

};


