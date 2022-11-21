
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


    content.justifyContent = juce::FlexBox::JustifyContent::center;
    content.alignContent = juce::FlexBox::AlignContent::center;
    content.flexDirection = juce::FlexBox::Direction::column;

    content.items.add(juce::FlexItem(playerPanel).withFlex(1).withMaxHeight(350).withMargin({ 21, 21, 14, 21 })); // withMinWidth(400.0f).withMinHeight(getHeight()).withMargin(20));
    content.items.add(juce::FlexItem(controllerPanel).withFlex(1).withMaxHeight(220).withMaxWidth(1600).withMargin({ 7, 21, 21, 21 }));

    panels.justifyContent = juce::FlexBox::JustifyContent::center;
    panels.alignContent = juce::FlexBox::AlignContent::center;

    panels.items.add(juce::FlexItem(*menu).withMinHeight(getHeight()).withMinWidth(39));
    panels.items.add(juce::FlexItem(content).withFlex(1));
    
}

SimulationEditor::~SimulationEditor() {

}

//==============================================================================
void SimulationEditor::paint(juce::Graphics& g) { }

void SimulationEditor::resized()
{
    content.performLayout(getLocalBounds().toFloat());
    panels.performLayout(getLocalBounds().toFloat());
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