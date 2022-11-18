
#include "SimulationEditor.h"
#include "SimulationState.h"

SimulationEditor::SimulationEditor(juce::AudioProcessor& owner)
    : AudioProcessorEditor(owner),
    volume("volume", "")
{
    addAndMakeVisible(menu = new MenuBar());
    volume.setSize(100, 25);
    volume.setSliderStyle(juce::Slider::LinearVertical);

    menu->addItemUp(audio);
    menu->addItemUp(volume);
    menu->addItemDown(settings);

    addAndMakeVisible(controllerPanel);
    addAndMakeVisible(playerPanel);

    panels.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    panels.alignContent = juce::FlexBox::AlignContent::center;

    panels.items.add(juce::FlexItem(*menu).withMinHeight(getHeight()).withMinWidth(39));
    panels.items.add(juce::FlexItem(controllerPanel).withFlex(1).withMargin(20));
    panels.items.add(juce::FlexItem(playerPanel).withMinWidth(400.0f).withMinHeight(getHeight()).withMargin(20));
}

SimulationEditor::~SimulationEditor() {

}

//==============================================================================
void SimulationEditor::paint(juce::Graphics& g) {
    //g.fillAll(juce::Colours::transparentBlack);
}

void SimulationEditor::resized()
{
    panels.performLayout(getLocalBounds().toFloat());

    repaint();
}

void SimulationEditor::setupSettingsModal(juce::AudioDeviceSelectorComponent* audioSettings) {
    auto wrapper = new Component();
    wrapper -> setSize(600,400);

    settings.onClick = [this,audioSettings,wrapper] {
        juce::DialogWindow::showDialog(
            "Settings",
            audioSettings,
            wrapper,
            getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
            true
        );
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