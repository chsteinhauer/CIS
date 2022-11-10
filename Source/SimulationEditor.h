
#pragma once
#include <JuceHeader.h>
#include "ParameterController.h"

struct MenuBar : juce::Component {
    MenuBar() {}
    ~MenuBar() {}

    // If item should be placed in the upper section of the menu bar
    void addItemUp(juce::Component& item) {
        addAndMakeVisible(item);
        itemsUp.add(&item);
    }

    // If item should be placed in the lower section of the menu bar
    void addItemDown(juce::Component& item) {
        addAndMakeVisible(item);
        itemsDown.add(&item);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter());
    }

    void resized() override {
        juce::FlexBox menu;
        menu.alignContent = juce::FlexBox::AlignContent::center;
        menu.flexDirection = juce::FlexBox::Direction::column;

        juce::FlexBox up;
        menu.justifyContent = juce::FlexBox::JustifyContent::flexStart;
        up.alignContent = juce::FlexBox::AlignContent::center;
        up.flexDirection = juce::FlexBox::Direction::column;


        for (int i = 0; i < itemsUp.size(); i++) {
            auto item = itemsUp[i];
            up.items.add(juce::FlexItem(*item)
                .withMinHeight(item->getWidth())
                .withMinWidth(item->getHeight())
                .withMargin({ 7,7,0,7 }));
        }

        juce::FlexBox down;
        down.alignContent = juce::FlexBox::AlignContent::center;
        down.flexDirection = juce::FlexBox::Direction::column;
        menu.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

        for (int i = 0; i < itemsDown.size(); i++) {
            auto item = itemsDown[i];
            down.items.add(juce::FlexItem(*item)
                .withMinHeight(item->getHeight())
                .withMinWidth(item->getWidth())
                .withMargin({ 0,7,7,7 }));
        }


        menu.items.add(juce::FlexItem(up).withFlex(1).withMargin({ 4,0,0,0 }));
        menu.items.add(juce::FlexItem(down).withMinHeight(39 * itemsDown.size()));

        menu.performLayout(getLocalBounds().toFloat());
    }


    juce::Array<juce::Component*> itemsUp;
    juce::Array<juce::Component*> itemsDown;
};


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

private: 


    // Menu bar and items

    MenuBar* menu;

    SettingsButton settings;
    AudioToggleButton audio;
    VolumeSlider volume;
};