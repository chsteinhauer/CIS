

#pragma once
#include <JuceHeader.h>
#include "SimulationState.h"
#include <random>
#include <gammatone/filter.hpp>


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

    std::mt19937 rng{ std::random_device{}() };
    std::normal_distribution<float> rand{ 0.0, 1.0 };
};

class PSHC {
public:
    PSHC();
    ~PSHC();

    void prepare(const juce::dsp::ProcessSpec& spec);
    float next(int channel);
    void reset();

private:

    struct HarmonicComplex {
        HarmonicComplex();
        HarmonicComplex(const HarmonicComplex&) = delete;
        HarmonicComplex& operator=(const HarmonicComplex&) = delete;
        HarmonicComplex(HarmonicComplex&&) = default;
        HarmonicComplex& operator=(HarmonicComplex&&) = default;

        void genHarmonics(float lo, float hi, float sampleRate);
        float getSample();
        void reset();

        const float f0 = 0.3f;
        int currentIndex = 0;

        std::vector<float> pshc;
        std::mt19937 rng{ std::random_device{}() };
    };

    std::vector<HarmonicComplex> pshcs;
};