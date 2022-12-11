
#pragma once
#include <JuceHeader.h>
#include "SpectrumVisualizer.h"
#include "MediaPlayerComponent.h"
#include "ChannelSpectrumVisualizer.h"
#include "ChannelFilterVisualizer.h"
#include "ControllerComponents.h"

class PlayerPanel : public juce::Component {
public:
    PlayerPanel();
    ~PlayerPanel() override;


    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    void mediaToggleButtonChanged();


	juce::FlexBox panel;
    juce::FlexBox player;

    juce::TextButton outToggle { "Change View" };
    ThresholdSlider threshold;
    SpectrumVisualizer IN;
    SpectrumVisualizer OUT;
    ChannelSpectrumVisualizer OUT_CHANNELS;

    CheckBox mediaToggle;
    MediaPlayerComponent mediaPlayer;
};