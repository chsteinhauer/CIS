

#include <JuceHeader.h>

#pragma once
class SpectrumVisualizer  : public juce::Component, private juce::Timer {
    enum
    {
        fftOrder = 12,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

    public:
        SpectrumVisualizer();
        ~SpectrumVisualizer();

        void timerCallback() override;

        void paint(juce::Graphics& g) override;
        void drawNextFrameOfSpectrum();
        void drawFrame(juce::Graphics& g);

        void setTitle(std::string str);
        void pushNextSampleIntoFifo(float sample) noexcept;

    private:
        juce::dsp::FFT forwardFFT;
        juce::dsp::WindowingFunction<float> window;

        float fifo[fftSize];
        float fftData[2 * fftSize];
        int fifoIndex = 0;
        bool nextFFTBlockReady = false;
        float scopeData[scopeSize];
        std::string title;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumVisualizer)
};