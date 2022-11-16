/*
  ==============================================================================

    EnvelopeExtractor.h
    Created: 14 Nov 2022 5:06:23pm
    Author:  Sønderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class EnvelopeExtractor
{
public:
    EnvelopeExtractor();

    void halfwaveRectification(juce::dsp::AudioBlock<float> block);
    void hilbertTransform(juce::dsp::AudioBlock<float> block);

private:

};
