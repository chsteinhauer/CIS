
#pragma once
#include <JuceHeader.h>
#include "SpectrumVisualizer.h"
#include "MediaPlayerComponent.h"
#include "ChannelSpectrumVisualizer.h"
#include "ChannelFilterVisualizer.h"

class PlayerPanel : public juce::Component {
public:
    PlayerPanel();
    ~PlayerPanel() override;

    void mediaToggleButtonChanged();

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

	juce::FlexBox panel;
    juce::FlexBox player;

    juce::TextButton outToggle { "View" };
    SpectrumVisualizer IN;
    SpectrumVisualizer OUT;
    ChannelSpectrumVisualizer OUT_CHANNELS;

    juce::ToggleButton mediaToggle;
    MediaPlayerComponent mediaPlayer;
};