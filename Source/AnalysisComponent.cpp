/*
  ==============================================================================

    AnalysisComponent.cpp
    Created: 13 Nov 2022 9:34:18am
    Author:  Sonderbo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AnalysisComponent.h"

//==============================================================================
AnalysisComponent::AnalysisComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(numChannels);
    numChannels.setInputRestrictions(2, "0123456789"); // Set to not allow above 99 channels
    numChannels.setText("Number of channels");
    numChannels.setMultiLine(false);
    numChannels.setTextToShowWhenEmpty("0", juce::Colours::white);

}

AnalysisComponent::~AnalysisComponent()
{
}

void AnalysisComponent::analyseAudioBlock(juce::dsp::AudioBlock<float> block)
{
    

}

void AnalysisComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("AnalysisComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AnalysisComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    numChannels.setBounds(0, 0, getWidth(), getHeight());
}
