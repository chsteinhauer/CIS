#pragma once

#include <JuceHeader.h>
#include "SpectrumVisualizer.h"
#include "SimulationEngine.h"
#include "PreprocessExample.h"
#include "AnalyseExample.h"
#include "ReconstructionExample.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    juce::AudioDeviceManager otherDeviceManager;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettings;
    std::unique_ptr<SimulationEngine<PreprocessExample, AnalyseExample, ReconstructionExample>> engine;


    SpectrumVisualizer IN;
    SpectrumVisualizer OUT;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
