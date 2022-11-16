

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

private:
    juce::Grid grid;

    juce::FlexBox checkBoxes;

    Select channels;
    CheckBox sine;
    CheckBox noise;

};