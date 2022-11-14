
#include "ControllerPanel.h"

ControllerPanel::ControllerPanel() : 
    sine("sine", "Sine processor"), 
    noise("noise","Noise processor")
    //channels("channelN","Select number of channels")
{
    addAndMakeVisible(sine);
    addAndMakeVisible(noise);
    //addAndMakeVisible(channels);

    checkBoxes.alignItems = juce::FlexBox::AlignItems::flexStart;
    checkBoxes.flexDirection = juce::FlexBox::Direction::column;

    checkBoxes.items.add(flexItem(&sine));
    checkBoxes.items.add(flexItem(&noise));
    //checkBoxes.items.add(flexItem(&channels));
}
ControllerPanel::~ControllerPanel() {}

void ControllerPanel::paint(juce::Graphics& g) {
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(0.1F));
    g.fillRoundedRectangle(0,0,getWidth(), getHeight(),10.0f);
}

void ControllerPanel::resized() {
    checkBoxes.performLayout(getLocalBounds().toFloat());
}

juce::FlexItem ControllerPanel::flexItem(juce::Component* item) {
    return juce::FlexItem(*item).withMinWidth(item->getWidth()).withMinHeight(item->getHeight()).withMargin({ 7,7,0,7 });
}