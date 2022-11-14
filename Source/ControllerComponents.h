
#pragma once
#include <JuceHeader.h>
#include "SimulationState.h"


struct CheckBox : public juce::ToggleButton {
    CheckBox(std::string id, std::string text);

    void mouseEnter(const juce::MouseEvent& evt) override;
    void mouseExit(const juce::MouseEvent& evt) override;

    juce::AudioProcessorValueTreeState::ButtonAttachment attachment;
};

struct SettingsButton : public juce::ImageButton {
    SettingsButton();
    ~SettingsButton();

    void mouseEnter(const juce::MouseEvent& evt) override;

    void mouseExit(const juce::MouseEvent& evt) override;
};

struct Select : public juce::ComboBox {
    Select(std::string id, std::string text);
    ~Select();

    void mouseEnter(const juce::MouseEvent& evt) override;

    void mouseExit(const juce::MouseEvent& evt) override;

    juce::AudioProcessorValueTreeState::ComboBoxAttachment attachment;
};

struct VolumeSlider : public juce::Slider {
    VolumeSlider();
    ~VolumeSlider();

    void mouseDown(const juce::MouseEvent& evt) override;

    void mouseUp(const juce::MouseEvent& evt) override;

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    juce::Image icon;
};

struct AudioToggleButton : public juce::ImageButton, juce::AudioProcessorValueTreeState::Listener {

    AudioToggleButton();
    ~AudioToggleButton();

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    void setState(juce::Image state);

    void toggleState();

    void mouseEnter(const juce::MouseEvent& evt) override;

    void mouseExit(const juce::MouseEvent& evt) override;

    juce::Image audio;
    juce::Image mute;

    juce::AudioProcessorValueTreeState::ButtonAttachment attachment;
};