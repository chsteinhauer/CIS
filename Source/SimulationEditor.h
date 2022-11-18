
#pragma once
#include <JuceHeader.h>
#include "ControllerComponents.h"
#include "MenuBar.h"
#include "ControllerPanel.h"
#include "SpectrumVisualizer.h"
#include "MediaPlayerComponent.h"
#include "PlayerPanel.h"




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


    juce::FlexBox panels;

    // Left panel: menu bar and menu items
    MenuBar* menu;

    SettingsButton settings;
    AudioToggleButton audio;
    VolumeSlider volume;

    // Middle panel: control components
    ControllerPanel controllerPanel;

    // Right panel: player & visuals
    PlayerPanel playerPanel;
 
};