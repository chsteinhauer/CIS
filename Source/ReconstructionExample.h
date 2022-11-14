
#pragma once

#include <JuceHeader.h>
#include "SimulationState.h"

class ReconstructionExample {
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    void reset();

    juce::dsp::Oscillator<float>* setupSinewave(int index, int N);
    void generateNoise();

private:
    std::vector<juce::dsp::Oscillator<float>*> oscillators;

    juce::HeapBlock<char> tempBlockMemory;
    juce::dsp::AudioBlock<float> tempBlock;

    juce::HeapBlock<char> noiseBlockMemory;
    juce::dsp::AudioBlock<float> noiseBlock;

    juce::Random random;
};


