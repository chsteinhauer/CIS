
#include "ControllerPanel.h"

ControllerPanel::ControllerPanel() : 
    sine("sine", "Sine processor"), 
    noise("noise","Noise processor"),
    channels("channelN","Select number of channels")
{
    addAndMakeVisible(sine);
    addAndMakeVisible(noise);
    addAndMakeVisible(channels);

    checkBoxes.alignItems = juce::FlexBox::AlignItems::flexStart;
    checkBoxes.flexDirection = juce::FlexBox::Direction::column;

    checkBoxes.items.add(flexItem(&channels).withMargin({24,7,0,7}));
    checkBoxes.items.add(flexItem(&sine));
    checkBoxes.items.add(flexItem(&noise));

    channelControllers.alignItems = juce::FlexBox::AlignItems::flexStart;
    channelControllers.flexDirection = juce::FlexBox::Direction::row;
    setupChannelControllers();

    panel.alignItems = juce::FlexBox::AlignItems::flexStart;
    panel.flexDirection = juce::FlexBox::Direction::column;
    panel.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    panel.items.add(juce::FlexItem(checkBoxes).withMargin(14).withFlex(1));
    panel.items.add(juce::FlexItem(channelControllers).withMargin(14).withFlex(1));//.withMinWidth(getWidth()-14).withMinHeight(getHeight()-28).withFlex(1));

    channels.onChange = [this] { resized(); };
}
ControllerPanel::~ControllerPanel() {}

void ControllerPanel::paint(juce::Graphics& g) {
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(0.1F));
    g.fillRoundedRectangle(0, 0, getWidth(), (getHeight() / static_cast<float>(2))-7, 10.0f);
    g.fillRoundedRectangle(0, (getHeight() / static_cast<float>(2)) + 7, getWidth(), (getHeight() / static_cast<float>(2)) - 7, 10.0f);
}

void ControllerPanel::resized() {

    int N = channels.getSelectedId();
    for (int i = 0; i < 100; i++) {
        sliders.at(i)->setVisible(i < N-1);
        sliders.at(i)->label.setVisible(i < N-1);
    }

    channelControllers.performLayout(getLocalBounds().toFloat());
    checkBoxes.performLayout(getLocalBounds().toFloat());

    panel.performLayout(getLocalBounds().toFloat());
}

juce::NormalisableRange<float> ControllerPanel::gainRange(float min, float max)
{
    return juce::NormalisableRange<float>(min, max,
        [=](float min, float, float v) { return juce::Decibels::decibelsToGain(v); },
        [=](float min, float, float v) { return juce::Decibels::gainToDecibels(v); }
    );
}

juce::FlexItem ControllerPanel::flexItem(juce::Component* item) {
    return juce::FlexItem(*item).withMinWidth(item->getWidth()).withMinHeight(item->getHeight()).withMargin({ 0,7,7,7 });
}

void ControllerPanel::setupChannelControllers() {
    for (int i = 0; i < 100; i++) {
        auto key = "channel" + std::to_string(i + 1);

        sliders.push_back(std::make_unique<VolumeSlider>(key, "C"+std::to_string(i + 1)));

        auto& slider = sliders.at(i);
        addChildComponent(slider.get());
        slider->setSliderStyle(juce::Slider::LinearVertical);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 30, 15);

        channelControllers.items.add(juce::FlexItem(*slider).withMinWidth(slider->getHeight()).withMinHeight(slider->getWidth()).withMargin({ 35,7,7,7 }));
        
    }
}