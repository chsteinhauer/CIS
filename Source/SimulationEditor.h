
#pragma once
#include <JuceHeader.h>
#include "ParameterController.h"

struct MenuBar : juce::Component {
    MenuBar() {}
    ~MenuBar() {}

    void addItem(juce::Component& item) {
        addAndMakeVisible(item);
        items.add(&item);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter());
    }

    void resized() override {
        juce::FlexBox menu;
        menu.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
        menu.alignContent = juce::FlexBox::AlignContent::center;
        menu.flexDirection = juce::FlexBox::Direction::column;

        for (int i = 0; i < items.size(); i++) {
            auto item = items[i];
            menu.items.add(juce::FlexItem(*item)
                .withMinHeight(item->getWidth())
                .withMinWidth(item->getHeight())
                .withMargin(7));
        }

        menu.performLayout(getLocalBounds().toFloat());
    }


    juce::Array<juce::Component*> items;
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
    IconController settings;
    SliderController gainSlider;

    MenuBar* menu;
};