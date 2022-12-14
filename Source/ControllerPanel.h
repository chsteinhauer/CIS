

#pragma once
#include <JuceHeader.h>
#include "ControllerComponents.h"
#include "MediaPlayerComponent.h"

class ControllerPanel : public juce::Component {
public:

    ControllerPanel();
    ~ControllerPanel();

    void paint(juce::Graphics& g) override;
    void resized() override;
    juce::FlexItem flexItem(juce::Component* item);
    juce::NormalisableRange<float> gainRange(float min, float max);
    void setupChannelControllers();

    juce::FlexBox panel;
    juce::FlexBox checkBoxes;
    juce::FlexBox channelControllers;
    juce::FlexBox config;
    

    Select channels;
    CheckBox sine;
    CheckBox noise;
    CheckBox pshc;

    VolumeSlider sineGain { "sinegain", "" };
    VolumeSlider noiseGain { "noisegain", "" };
    VolumeSlider pshcGain { "pshcgain", "" };

    //FrequencySlider fmin;
    FrequencySlider fslider;
    std::vector<std::unique_ptr<VolumeSlider>> sliders;
};