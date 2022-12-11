

#include "ControllerComponents.h"


/* CheckBox */
CheckBox::CheckBox(std::string id, std::string text) : attachment(*State::GetInstance(), id, *this) {
    setButtonText(text);
    setSize(200, 30);
}

void CheckBox::mouseEnter(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    juce::ToggleButton::mouseEnter(evt);
}

void CheckBox::mouseExit(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::ToggleButton::mouseExit(evt);
}

/* SettingsButton */
SettingsButton::SettingsButton() {
    auto colour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(2.1F);

    juce::Image icon = juce::ImageCache::getFromMemory(BinaryData::settings24_png, BinaryData::settings24_pngSize);
    setImages(true, false, true, icon, 1.0, colour, icon, 1.0, colour.brighter(), icon, 1.0, colour);
}

SettingsButton::~SettingsButton() {};

void SettingsButton::mouseEnter(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    juce::ImageButton::mouseEnter(evt);
}

void SettingsButton::mouseExit(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::ImageButton::mouseExit(evt);
}

Select::Select(std::string id, std::string _label = "") : juce::ComboBox(id), attachment(*State::GetInstance(), id, *this) {
   // auto par = State::GetInstance()->getParameter(id);
    auto items = State::GetAllValueStrings(id);
    auto value = State::GetDenormalizedValue(id);

    addItemList(items, 1);
    setSize(200, 30);

    if (_label != "") {
        label.setText(_label, juce::dontSendNotification);
        label.attachToComponent(this, false);
    }

    addAndMakeVisible(label);
    setSelectedId(6);
}

Select::~Select() {}

void Select::mouseEnter(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    juce::ComboBox::mouseEnter(evt);
}

void Select::mouseExit(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::ComboBox::mouseExit(evt);
}

/* VolumeSlider */
VolumeSlider::VolumeSlider(std::string id, std::string _label) : attachment(*State::GetInstance(), id, *this) {
    auto par = State::GetInstance()->getParameterRange(id);

    // default slider config
    setRange(par.start, par.end, par.interval);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setSize(150, 50);

    if (!_label.empty()) {
        label.setText(_label, juce::dontSendNotification);
        label.attachToComponent(this, false);
    }

    addAndMakeVisible(label);
}

VolumeSlider::~VolumeSlider() {}

void VolumeSlider::mouseDown(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDown(evt);
}

void VolumeSlider::mouseUp(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(evt);
}


/* VolumeSlider */
ThresholdSlider::ThresholdSlider(std::string id, std::string _label) : attachment(*State::GetInstance(), id, *this) {
    auto par = State::GetInstance()->getParameterRange(id);

    // default slider config
    setRange(par.start, par.end, par.interval);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setSize(190, 30);
    setSliderStyle(juce::Slider::LinearVertical);

    if (!_label.empty()) {
        label.setText(_label, juce::dontSendNotification);
        label.attachToComponent(this, false);
    }

    addAndMakeVisible(label);
}

ThresholdSlider::~ThresholdSlider() {}

void ThresholdSlider::mouseDown(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDown(evt);
}

void ThresholdSlider::mouseUp(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(evt);
}


/* FrequencySlider */
FrequencySlider::FrequencySlider(std::string _label) { //: attachment(*State::GetInstance(), id, *this) {
    setSliderStyle(juce::Slider::TwoValueHorizontal);
    setRange(20, 20000, 0.1f);

    // default slider config
    setSize(200, 75);
    setTextBoxStyle(juce::Slider::TextBoxBelow,true,200,25);

    if (!_label.empty()) {
        label.setText(_label, juce::dontSendNotification);
        label.attachToComponent(this, false);
    }

    addAndMakeVisible(label);

    setMinAndMaxValues(250, 4500);

    State::GetInstance()->addParameterListener("fmin", this);
    State::GetInstance()->addParameterListener("fmax", this);

    onValueChange = [this] { updateValueTree(); };
}

void FrequencySlider::parameterChanged(const juce::String& parameterID, float newValue) {
    /*if (parameterID == "fmin") {
        setMinValue(newValue);
    } else if (parameterID == "fmax") {
        setMaxValue(newValue);
    }*/
}

void FrequencySlider::updateValueTree() {
    auto fmin = State::GetInstance()->getParameter("fmin");
    auto minValue = fmin->convertTo0to1(getMinValue());
    fmin->setValueNotifyingHost(minValue);

    auto fmax = State::GetInstance()->getParameter("fmax");
    auto maxValue = fmax->convertTo0to1(getMaxValue());
    fmax->setValueNotifyingHost(maxValue);

    updateText();
}

juce::String FrequencySlider::getTextFromValue(double value) {

    return juce::String(getMinValue()) + " Hz - " + juce::String(getMaxValue()) + " Hz";
}

FrequencySlider::~FrequencySlider() {}

void FrequencySlider::mouseDown(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    juce::Slider::mouseDown(evt);
}

void FrequencySlider::mouseUp(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseUp(evt);
}


/* AudioToggleButton */

AudioToggleButton::AudioToggleButton() : attachment(*State::GetInstance(), "audio", *this) {
    audio = juce::ImageCache::getFromMemory(BinaryData::audio24_png, BinaryData::audio24_pngSize);
    mute = juce::ImageCache::getFromMemory(BinaryData::noaudio24_png, BinaryData::noaudio24_pngSize);

    setClickingTogglesState(true);
    setState(audio);

    onStateChange = [this] { toggleState(); };

    State::GetInstance()->addParameterListener("volume", this);
}
AudioToggleButton::~AudioToggleButton() {}

void AudioToggleButton::parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == "volume" && getToggleState()) {
        setToggleState(false, juce::dontSendNotification);
        State::GetInstance()->getParameter("audio")->setValue(0);

        setState(audio);
    }
}

void AudioToggleButton::setState(juce::Image state) {
    auto colour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(2.1F);

    setImages(true, false, true, state, 1.0, colour, state, 1.0, colour.brighter(), state, 1.0, colour);
}

void AudioToggleButton::toggleState() {
    auto isMuted = getToggleState();

    setState(isMuted ? mute : audio);
}

void AudioToggleButton::mouseEnter(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    juce::ImageButton::mouseEnter(evt);
}

void AudioToggleButton::mouseExit(const juce::MouseEvent& evt) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::ImageButton::mouseExit(evt);
}
