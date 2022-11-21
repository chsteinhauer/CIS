

#include <JuceHeader.h>
#include "SimulationState.h"

#pragma once

class ChannelSpectrumVisualizer : public juce::Component, public juce::Timer {
    enum
    {
        fftOrder = 9,
        fftSize = 1 << fftOrder,
        scopeSize = 512,
    };

public:
    ChannelSpectrumVisualizer();
    ~ChannelSpectrumVisualizer();

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void drawNextMultiFrameOfSpectrum(int channel);
    void drawFrame(juce::Graphics& g);

    void setTitle(std::string str);
    void pushNextSampleIntoFifoTable(int channel, float sample) noexcept;

private:
    const static int maxN = 15;//State::maxNumChannels,

    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    std::string title;

    float fftTable[maxN][2 * fftSize] = {{0}};
    float fifoTable[maxN][2 * fftSize] = {{0}};
    int index[maxN] = { 0 };
    bool blockReady[maxN] = { false };
    float multiScopeData[maxN][scopeSize] = {{0}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelSpectrumVisualizer)
};