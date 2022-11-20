
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel() {
    // Add input and output spectrum visualizer 
    addAndMakeVisible(IN);
    IN.setTitle("Input");
    addAndMakeVisible(OUT);
    OUT.setTitle("Output");
    addChildComponent(OUT_CHANNELS);
    OUT_CHANNELS.setTitle("Channels Output");

    outToggle.onClick = [this] { resized(); };
    outToggle.setClickingTogglesState(true);
    addAndMakeVisible(outToggle);
    
    addAndMakeVisible(mediaPlayer);
    addAndMakeVisible(mediaToggle);
    mediaToggle.setButtonText("Use media as input");
    mediaToggle.onClick = [this] { mediaToggleButtonChanged(); };
    mediaPlayer.setEnabled(mediaToggle.getToggleState());

    // Player panel
    player.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    player.alignContent = juce::FlexBox::AlignContent::center;
    player.flexDirection = juce::FlexBox::Direction::column;

    player.items.add(juce::FlexItem(mediaToggle).withMinWidth(150).withMinHeight(20));
    player.items.add(juce::FlexItem(mediaPlayer).withMinWidth(300).withMinHeight(160));

    panel.flexDirection = juce::FlexBox::Direction::row;
    panel.alignContent = juce::FlexBox::AlignContent::flexEnd;


    // Media Player
    panel.items.add(juce::FlexItem(IN).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withFlex(1).withMargin(7));
    panel.items.add(juce::FlexItem(OUT).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withFlex(1).withMargin(7));
    //panel.items.add(juce::FlexItem(OUT_CHANNELS).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withFlex(1).withMargin(7));
    panel.items.add(juce::FlexItem(player).withMinWidth(300).withMinHeight(200.0f).withMargin(7));      


}

PlayerPanel::~PlayerPanel() {}

void PlayerPanel::paint(juce::Graphics& g) {

}

void PlayerPanel::resized()
{
    panel.items.clearQuick();

    IN.stopTimer();
    OUT.stopTimer();
    OUT_CHANNELS.stopTimer();

    panel.items.add(juce::FlexItem(IN).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withFlex(1).withMargin(7));
    juce::Rectangle<int> outBounds;

    IN.startTimerHz(30);
    if (!outToggle.getToggleStateValue().getValue()) {
        panel.items.add(juce::FlexItem(OUT).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withFlex(1).withMargin(7));

        OUT_CHANNELS.setVisible(false);
        OUT.setVisible(true);
        OUT.startTimerHz(30);
    }
    else 
    {
        panel.items.add(juce::FlexItem(OUT_CHANNELS).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withFlex(1).withMargin(7));

        OUT.setVisible(false);
        OUT_CHANNELS.setVisible(true);
        OUT_CHANNELS.startTimerHz(30);
    }

    panel.items.add(juce::FlexItem(player).withMinWidth(300).withMinHeight(200.0f).withMargin(7));

    player.performLayout(getLocalBounds().toFloat());
    panel.performLayout(getLocalBounds().toFloat());

    auto p = getLocalBounds().getTopRight();
    outToggle.setBounds(p.getX()-370,p.getY()+15,40,20);
}

void PlayerPanel::mediaToggleButtonChanged()
{
    mediaPlayer.setEnabled(mediaToggle.getToggleState());
    if (mediaToggle.getToggleState() == false)
    {
        mediaPlayer.stopMediaPlayer();
    }
}