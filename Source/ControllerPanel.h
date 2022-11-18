

#pragma once
#include <JuceHeader.h>
#include "ControllerComponents.h"

class ControllerPanel : public juce::Component {
public:

    ControllerPanel();
    ~ControllerPanel();

    void paint(juce::Graphics& g) override;
    void resized() override;
    juce::FlexItem flexItem(juce::Component* item);
    juce::NormalisableRange<float> gainRange(float min, float max);
    void setupChannelControllers();

private:
    juce::FlexBox panel;

    juce::FlexBox checkBoxes;
    juce::FlexBox channelControllers;

    Select channels;
    CheckBox sine;
    CheckBox noise;
    std::vector<std::unique_ptr<VolumeSlider>> sliders;
};