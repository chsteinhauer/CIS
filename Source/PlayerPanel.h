
#pragma once
#include <JuceHeader.h>
#include "SpectrumVisualizer.h"
#include "MediaPlayerComponent.h"

class PlayerPanel : public juce::Component {
public:
    PlayerPanel();
    ~PlayerPanel() override;

    void mediaToggleButtonChanged();

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

	juce::FlexBox panel;

    SpectrumVisualizer IN;
    SpectrumVisualizer OUT;

    juce::ToggleButton mediaToggle;
    MediaPlayerComponent mediaPlayer;
};