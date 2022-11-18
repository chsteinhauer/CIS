
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel() {
    // Add input and output spectrum visualizer 
    addAndMakeVisible(IN);
    IN.setTitle("Input");
    addAndMakeVisible(OUT);
    OUT.setTitle("Output");

    addAndMakeVisible(mediaPlayer);

    addAndMakeVisible(mediaToggle);
    mediaToggle.setButtonText("Use media as input");
    mediaToggle.onClick = [this] { mediaToggleButtonChanged(); };
    mediaPlayer.setEnabled(mediaToggle.getToggleState());
}

PlayerPanel::~PlayerPanel() {}

void PlayerPanel::paint(juce::Graphics& g) {

}

void PlayerPanel::resized()
{
    // Player panel
    panel.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    panel.alignContent = juce::FlexBox::AlignContent::center;
    panel.flexDirection = juce::FlexBox::Direction::column;

    // Media Player
    panel.items.add(juce::FlexItem(mediaToggle).withMinWidth(150).withMinHeight(20));
    panel.items.add(juce::FlexItem(mediaPlayer).withMinWidth(400).withMinHeight(160));
    panel.items.add(juce::FlexItem(IN).withMinWidth(300.0f).withMinHeight(200.0f));
    panel.items.add(juce::FlexItem(OUT).withMinWidth(300.0f).withMinHeight(200.0f));

    panel.performLayout(getLocalBounds().toFloat());
}

void PlayerPanel::mediaToggleButtonChanged()
{
    mediaPlayer.setEnabled(mediaToggle.getToggleState());
    if (mediaToggle.getToggleState() == false)
    {
        mediaPlayer.stopMediaPlayer();
    }
}