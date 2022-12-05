
#include "ControllerPanel.h"

ControllerPanel::ControllerPanel() : 
    sine("sine", "Sine processor"), 
    noise("noise","Noise processor"),
    pshc("pshc", "PSHC processor"),
    channels("channelN","Select number of channels"),
    fslider("Frequency range")
{
    addAndMakeVisible(sine);
    addAndMakeVisible(noise);
    addAndMakeVisible(pshc);
    addAndMakeVisible(channels);
    addAndMakeVisible(fslider);

    freqSliders.alignItems = juce::FlexBox::AlignItems::flexStart;
    freqSliders.justifyContent = juce::FlexBox::JustifyContent::center;

    freqSliders.items.add(flexItem(&fslider).withMargin(7));

    checkBoxes.alignItems = juce::FlexBox::AlignItems::flexStart;
    checkBoxes.flexDirection = juce::FlexBox::Direction::column;
    checkBoxes.justifyContent = juce::FlexBox::JustifyContent::center;

    checkBoxes.items.add(flexItem(&channels).withMargin({18,7,0,7}));
    checkBoxes.items.add(flexItem(&sine));
    checkBoxes.items.add(flexItem(&noise));
    checkBoxes.items.add(flexItem(&pshc));

    channelControllers.alignItems = juce::FlexBox::AlignItems::center;
    channelControllers.flexDirection = juce::FlexBox::Direction::row;
    setupChannelControllers();

    panel.alignItems = juce::FlexBox::AlignItems::center;
    panel.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    panel.items.add(juce::FlexItem(checkBoxes).withMinHeight(200).withMinWidth(200).withMargin(14));
    panel.items.add(juce::FlexItem(freqSliders).withMinHeight(100).withMinWidth(200).withMargin(14));
    panel.items.add(juce::FlexItem(channelControllers).withFlex(1).withMaxHeight(200).withMargin(14));

    channels.onChange = [this] { resized(); };
}
ControllerPanel::~ControllerPanel() {}

void ControllerPanel::paint(juce::Graphics& g) {
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(0.1F));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 10.0f);

    // Rectangles around the sliders
    
    for (int i = 0; i < std::max(15,(int)State::GetDenormalizedValue("channelN")); i++) {
        auto r = sliders.at(i)->getBounds();

        g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).darker(0.1F));
        g.fillRoundedRectangle(r.getX(),r.getY(),r.getWidth(),r.getHeight()+5, 10.0f);

        if ((int)State::GetDenormalizedValue("channelN") <= i) continue;
        
        g.setColour(juce::Colours::white);
        g.setOpacity(0.8);
        g.fillRoundedRectangle(r.getX() + 32.5, r.getY() - 17.5, 11, 11, 2.5f);
        g.setOpacity(1);
        g.setColour(State::colours[i]);
        g.fillRoundedRectangle(r.getX() + 33, r.getY() - 17, 10, 10, 2.f);
    }
}

void ControllerPanel::resized() {
    int N = channels.getSelectedId();
    for (int i = 0; i < State::maxNumChannels; i++) {
        sliders.at(i)->setVisible(i < N-1);
        sliders.at(i)->label.setVisible(i < N-1);
    }

    freqSliders.performLayout(getLocalBounds().toFloat());
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
    return juce::FlexItem(*item).withMinWidth(item->getWidth()).withMinHeight(item->getHeight()).withMargin({ 7,7,0,7 });
}

void ControllerPanel::setupChannelControllers() {
    for (int i = 0; i < State::maxNumChannels; i++) {
        auto key = "channel" + std::to_string(i + 1);

        sliders.push_back(std::make_unique<VolumeSlider>(key, "C"+std::to_string(i + 1)));

        auto& slider = sliders.at(i);
        addChildComponent(slider.get());
        slider->setColour(juce::Slider::backgroundColourId, findColour(juce::ResizableWindow::backgroundColourId).darker(0.1F));
        slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        slider->setSliderStyle(juce::Slider::LinearVertical);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 36, 15);

        channelControllers.items.add(juce::FlexItem(*slider).withMinWidth(slider->getHeight()).withMinHeight(slider->getWidth()).withMargin(7));
        
    }
}
