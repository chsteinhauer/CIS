/*
  ==============================================================================

    EnvelopeExtractor.cpp
    Created: 14 Nov 2022 5:06:23pm
    Author:  Sønderbo

  ==============================================================================
*/

#include "EnvelopeExtractor.h"

EnvelopeExtractor::EnvelopeExtractor()
{

}

void EnvelopeExtractor::halfwaveRectification(juce::dsp::AudioBlock<float> block)
{
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
}

void EnvelopeExtractor::hilbertTransform(juce::dsp::AudioBlock<float> block)
{

}