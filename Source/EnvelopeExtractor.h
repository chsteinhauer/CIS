

#pragma once

#include <JuceHeader.h>

class EnvelopeExtractor
{
    enum
    {
        fftOrder = 10,
        fftSize = 1 << fftOrder
    };
public:
    EnvelopeExtractor();

    void halfwaveRectification(const juce::dsp::ProcessContextReplacing<float>& context);
    void hilbertTransform(juce::dsp::AudioBlock<float> block);
    void prepareHalfwaveRectification(const juce::dsp::ProcessSpec& spec);

private:

    //juce::dsp::FFT forwardFFT;
    //juce::dsp::WindowingFunction<float> window;

    //float fifo[fftSize];
    //float fftData[2 * fftSize];

     

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> iir;
};
