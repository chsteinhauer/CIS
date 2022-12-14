
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"
#include "ButterworthBandpass.h"
#include "Carrier.h"

using Block = juce::dsp::AudioBlock<float>;

class Reconstruction : juce::AudioProcessorValueTreeState::Listener {
public:
    Reconstruction();
    ~Reconstruction();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    void reset();

    void parameterChanged(const juce::String& parameterID, float newValue);

private:
    struct Synthesis {
        Synthesis();
        ~Synthesis();

        void prepare(const juce::dsp::ProcessSpec& spec);
        void process(const juce::dsp::ProcessContextReplacing<float>& context);
        void reset();

        Sine sine;
        Noise noise;
        PSHC pshc;
        juce::dsp::Gain<float> gain;
    };

    ButterworthBandpass butterworth;
    Synthesis synth;
  /*  juce::dsp::Compressor<float> compressor;
    juce::dsp::Gain<float> makeupGain;
    juce::dsp::NoiseGate<float> expander;*/
    
    bool initialized = false;
};


