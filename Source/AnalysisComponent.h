
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AnalysisComponent  : public juce::Component
{
public:
    AnalysisComponent();
    ~AnalysisComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void analyseAudioBlock(juce::dsp::AudioBlock<float> block);
private:

    juce::TextEditor numChannels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalysisComponent)
};
