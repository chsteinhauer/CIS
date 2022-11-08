/*
  ==============================================================================

    SimulationEditor.cpp
    Created: 3 Nov 2022 9:29:04am
    Author:  chris

  ==============================================================================
*/

#include "SimulationEditor.h"
#include "SimulationState.h"

SimulationEditor::SimulationEditor(juce::AudioProcessor& owner)
    : AudioProcessorEditor(owner),
    buttonAttachment(*State::GetInstance(), "settings", settings),
    gainSliderAttachment(*State::GetInstance(), "gain", gainSlider)
{
    addAndMakeVisible(gainSlider);
    gainSlider.setRange(0.0f, 1.0f);
    gainSlider.setSliderStyle(juce::Slider::Rotary);

    addAndMakeVisible(settings);
    settings.setClickingTogglesState(true);

    juce::Logger::outputDebugString("Yoooooooooooo-------");
}

SimulationEditor::~SimulationEditor() {

}

//void SimulationEditor::updateToggleState(juce::Button* button, juce::String name)
//{
//    auto state = button->getToggleState();
//}

//void SimulationEditor::setCallback(void (*callback)(void)) {
//    button.onClick = [this,callback] { callback(&button, "Gate"); };
//}

//==============================================================================
void SimulationEditor::paint(juce::Graphics& g) {
    //g.fillAll(juce::Colours::transparentBlack);
}

void SimulationEditor::resized()
{
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    fb.flexDirection = juce::FlexBox::Direction::column;

    fb.items.add(juce::FlexItem(settings).withMinWidth(100.0f).withMinHeight(50.0f));
    fb.items.add(juce::FlexItem(gainSlider).withMinWidth(200.0f).withMinHeight(100.0f));

    fb.performLayout(getLocalBounds().toFloat());

    repaint();
}


void SimulationEditor::timerCallback()
{
}

void SimulationEditor::hostMIDIControllerIsAvailable(bool controllerIsAvailable)
{
}

int SimulationEditor::getControlParameterIndex(Component& control)
{
    return 0;
}

void SimulationEditor::updateTrackProperties()
{

}

// called when the stored window size changes
void SimulationEditor::valueChanged(juce::Value&)
{

}