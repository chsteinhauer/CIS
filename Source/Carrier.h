

#pragma once
#include <JuceHeader.h>
#include "SimulationState.h"
#include <random>


class Sine {
public:
    Sine();
    ~Sine();

    void prepare(const juce::dsp::ProcessSpec& spec);
    float next(int channel);
    void reset();


private:

    // WavetableOscillator is generously copy & pasted from https://thewolfsound.com/sound-synthesis/wavetable-synth-plugin-in-juce/
    class WavetableOscillator
    {
    public:
        WavetableOscillator(std::vector<float> waveTable, double sampleRate);
        WavetableOscillator(const WavetableOscillator&) = delete;
        WavetableOscillator& operator=(const WavetableOscillator&) = delete;
        WavetableOscillator(WavetableOscillator&&) = default;
        WavetableOscillator& operator=(WavetableOscillator&&) = default;

        float getSample(); 
        void setFrequency(float frequency); 

    private:
        float interpolateLinearly() const;

        float index = 0.f; 
        float indexIncrement = 0.f; 
        std::vector<float> waveTable; 
        double sampleRate;
    };

    std::vector<float> generateSineWaveTable();

    const int OSCILLATOR_COUNT = State::maxNumChannels;
    const std::vector<float> sineWaveTable = generateSineWaveTable(); 
    std::vector<WavetableOscillator> oscillators;
};

class Noise {
public:
    Noise();
    ~Noise();

    float next();

    juce::Random random;
};

class PSHC {
public:
    PSHC();
    ~PSHC();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    void reset();
    float genHarmonic(int k);

    std::default_random_engine gen{};
    std::vector<int> r;
    std::uniform_int_distribution<int> u{ 0,1 };

    const float f0 = 0.3f;
};