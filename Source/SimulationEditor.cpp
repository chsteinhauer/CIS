

#include "SimulationEditor.h"
#include "SimulationState.h"

SimulationEditor::SimulationEditor(juce::AudioProcessor& owner)
    : AudioProcessorEditor(owner),
    gainSlider("volume","Volume",0.0f,1.0f),
    settings("settings","Settings",BinaryData::settings24_png,BinaryData::settings24_pngSize)
{
    addAndMakeVisible(menu = new MenuBar());
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);

    menu->addItem(gainSlider);
    menu->addItem(settings);
}

SimulationEditor::~SimulationEditor() {

}

//==============================================================================
void SimulationEditor::paint(juce::Graphics& g) {
    //g.fillAll(juce::Colours::transparentBlack);
}

void SimulationEditor::resized()
{
    juce::FlexBox editorPanels;

    editorPanels.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    editorPanels.alignContent = juce::FlexBox::AlignContent::center;

    editorPanels.items.add(juce::FlexItem(*menu).withMinHeight(getHeight()).withMinWidth(39));
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