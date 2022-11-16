
#pragma once
#include <JuceHeader.h>
#include "ControllerComponents.h"
#include "MenuBar.h"
#include "ControllerPanel.h"




class SimulationEditor : public juce::AudioProcessorEditor
{
public:
    SimulationEditor(juce::AudioProcessor& owner);
    ~SimulationEditor() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void hostMIDIControllerIsAvailable(bool controllerIsAvailable) override;
    int getControlParameterIndex(juce::Component& control) override;
    void updateTrackProperties();
    void setupSettingsModal(juce::AudioDeviceSelectorComponent* audioSettings);

private: 
    ControllerPanel ctlrPanel;

    MenuBar* menu;

    SettingsButton settings;
    AudioToggleButton audio;
    VolumeSlider volume;
};