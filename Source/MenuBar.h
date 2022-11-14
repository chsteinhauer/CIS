#pragma once
#include <JuceHeader.h>

struct MenuBar : juce::Component {
    MenuBar();
    ~MenuBar();

    // If item should be placed in the upper section of the menu bar
    void addItemUp(juce::Component& item);
    // If item should be placed in the lower section of the menu bar
    void addItemDown(juce::Component& item);
    void paint(juce::Graphics& g) override;
    void resized() override;


    juce::Array<juce::Component*> itemsUp;
    juce::Array<juce::Component*> itemsDown;
};