/*
  ==============================================================================

    EnvelopeExtractor.cpp
    Created: 14 Nov 2022 5:06:23pm
    Author:  Sønderbo

  ==============================================================================
*/

#include "EnvelopeExtractor.h"
#include "SimulationState.h"

EnvelopeExtractor::EnvelopeExtractor() 
{
    
}

void EnvelopeExtractor::prepareHalfwaveRectification(const juce::dsp::ProcessSpec& spec) {
    filters.clear();
    auto range = State::GetInstance()->getParameter("Greenwood");

    int N = spec.numChannels;
    float freq = range->convertFrom0to1(0);
    for (int i = 1; i <= N; i++) {
        auto coeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(freq, spec.sampleRate, 2);

        filters.push_back(std::make_unique<juce::dsp::IIR::Filter<float>>(coeffs[0]));

        freq = range->convertFrom0to1(static_cast<float>(i) / N);
    }
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

        filters.at(i)->process(juce::dsp::ProcessContextReplacing<float>(block.getSingleChannelBlock(i)));
    }
}

void EnvelopeExtractor::hilbertTransform(juce::dsp::AudioBlock<float> block)
{

}