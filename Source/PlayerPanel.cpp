
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel(): mediaToggle("togglemedia","Use media as input") {
    addAndMakeVisible(mediaPlayer);
    addAndMakeVisible(mediaToggle);
    //mediaToggle.setButtonText("Use media as input");
    mediaToggle.onClick = [this] { mediaToggleButtonChanged(); };
    mediaPlayer.setEnabled(mediaToggle.getToggleState());

    // Player panel
    player.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    player.alignContent = juce::FlexBox::AlignContent::center;
    player.flexDirection = juce::FlexBox::Direction::column;

    player.items.add(juce::FlexItem(mediaToggle).withMinWidth(150).withMinHeight(20).withMargin({7,0,7,0}));
    player.items.add(juce::FlexItem(mediaPlayer).withMinWidth(300).withMinHeight(160));

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
    
    panel.flexDirection = juce::FlexBox::Direction::row;
    panel.alignContent = juce::FlexBox::AlignContent::flexEnd;
}

PlayerPanel::~PlayerPanel() {}

void PlayerPanel::paint(juce::Graphics& g) {
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(0.1F));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10.0f);
}

void PlayerPanel::resized()
{
    panel.items.clearQuick();

    IN.stopTimer();
    OUT.stopTimer();
    OUT_CHANNELS.stopTimer();

    panel.items.add(juce::FlexItem(player).withMinWidth(300).withMinHeight(200.0f).withMargin({ 21, 14, 21, 21 }));
    panel.items.add(juce::FlexItem(IN).withFlex(1).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withMargin({ 21, 7, 21, 7 }));

    IN.startTimerHz(20);
    if (!outToggle.getToggleStateValue().getValue()) {
        panel.items.add(juce::FlexItem(OUT).withFlex(1).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withMargin({ 21, 21, 21, 14 }));

        OUT_CHANNELS.setVisible(false);
        OUT.setVisible(true);
        OUT.startTimerHz(20);
    }
    else
    {
        panel.items.add(juce::FlexItem(OUT_CHANNELS).withFlex(1).withMinWidth(300.0f).withMinHeight(200.0f).withMaxHeight(400).withMargin({ 21, 21, 21, 14 }));

        OUT.setVisible(false);
        OUT_CHANNELS.setVisible(true);
        OUT_CHANNELS.startTimerHz(20);
    }


    player.performLayout(getLocalBounds().toFloat());
    panel.performLayout(getLocalBounds().toFloat());

    auto p = getLocalBounds().getTopRight();
    outToggle.setBounds(p.getX()-110,p.getY()+30,80,20);
}


void PlayerPanel::mediaToggleButtonChanged()
{
    mediaPlayer.setEnabled(mediaToggle.getToggleState());
    if (mediaToggle.getToggleState() == false)
    {
        mediaPlayer.stopMediaPlayer();
    }
}
