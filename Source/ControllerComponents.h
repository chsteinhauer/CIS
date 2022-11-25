
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

    juce::Label label;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment attachment;
};

struct VolumeSlider : public juce::Slider {
    VolumeSlider(std::string id, std::string _label);
    ~VolumeSlider();

    void mouseDown(const juce::MouseEvent& evt) override;
    void mouseUp(const juce::MouseEvent& evt) override;

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    juce::Label label;
};

struct FrequencySlider : public juce::Slider {
    FrequencySlider(std::string id, std::string _label);
    ~FrequencySlider();

    void mouseDown(const juce::MouseEvent& evt) override;
    void mouseUp(const juce::MouseEvent& evt) override;
    juce::String getTextFromValue(double value) override;

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    juce::Label label;
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