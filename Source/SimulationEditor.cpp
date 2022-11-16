
#include "SimulationEditor.h"
#include "SimulationState.h"

SimulationEditor::SimulationEditor(juce::AudioProcessor& owner)
    : AudioProcessorEditor(owner)
{
    addAndMakeVisible(menu = new MenuBar());
    volume.setSliderStyle(juce::Slider::LinearVertical);

    menu->addItemUp(audio);
    menu->addItemUp(volume);
    menu->addItemDown(settings);

    addAndMakeVisible(ctlrPanel);
    //addAndMakeVisible(sine);
    //addAndMakeVisible(noise);
}

SimulationEditor::~SimulationEditor() {

}

//==============================================================================
void SimulationEditor::paint(juce::Graphics& g) {
    //g.fillAll(juce::Colours::transparentBlack);
}

void SimulationEditor::resized()
{
 /*   juce::FlexBox controllerPanel;

    controllerPanel.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    controllerPanel.alignContent = juce::FlexBox::AlignContent::center;
    controllerPanel.flexDirection = juce::FlexBox::Direction::column;

    controllerPanel.items.add(juce::FlexItem(sine).withMinWidth(sine.getWidth()).withMinHeight(sine.getHeight()).withMargin(7));
    controllerPanel.items.add(juce::FlexItem(noise).withMinWidth(noise.getWidth()).withMinHeight(noise.getHeight()).withMargin(7));*/

    juce::FlexBox editorPanels;

    editorPanels.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    editorPanels.alignContent = juce::FlexBox::AlignContent::center;

    editorPanels.items.add(juce::FlexItem(*menu).withMinHeight(getHeight()).withMinWidth(39));
    editorPanels.items.add(juce::FlexItem(ctlrPanel).withFlex(1).withMargin(20));
    editorPanels.performLayout(getLocalBounds().toFloat());

    repaint();
}

void SimulationEditor::setupSettingsModal(juce::AudioDeviceSelectorComponent* audioSettings) {
    auto wrapper = new Component();
    wrapper -> setSize(600,400);

    settings.onClick = [this,audioSettings,wrapper] {
        juce::DialogWindow::showDialog("Settings",audioSettings,wrapper,getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),true);
    };
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