/*
  ==============================================================================

    EnvelopeExtractor.cpp
    Created: 14 Nov 2022 5:06:23pm
    Author:  Sønderbo

  ==============================================================================
*/

#include "EnvelopeExtractor.h"

EnvelopeExtractor::EnvelopeExtractor() //: forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    
}

void EnvelopeExtractor::prepareHalfwaveRectification(const juce::dsp::ProcessSpec& spec) {
    iir.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, 250);
    iir.prepare(spec);
}

void EnvelopeExtractor::halfwaveRectification(const juce::dsp::ProcessContextReplacing<float>& context)
{
    
    juce::dsp::AudioBlock<float> block(context.getOutputBlock());

    for (int i = 0; i < block.getNumChannels(); i++)
    {
        for (int j = 0; j < block.getNumSamples(); j++)
        {
            auto sample = block.getSample(i, j);
            if (sample < 0)
            {
                block.setSample(i, j, 0);
            }
            else
            {
                block.setSample(i, j, sample);
            }
        }
    }

    iir.process(context);
}

void EnvelopeExtractor::hilbertTransform(juce::dsp::AudioBlock<float> block)
{

}