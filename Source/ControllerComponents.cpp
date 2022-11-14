

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

Select::Select(std::string id, std::string text) : juce::ComboBox(id), attachment(*State::GetInstance(), id, *this) {
    auto options = State::GetInstance()->getParameter(id);
    auto items = options->getAllValueStrings();
    auto defVal = (int)options->getDefaultValue();

    addItemList(items, defVal);//(int)options->getDefaultValue());
    setSize(200, 30);
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
VolumeSlider::VolumeSlider() : attachment(*State::GetInstance(), "volume", *this) {
    auto par = State::GetInstance()->getParameterRange("volume");

    // default slider config
    setRange(par.start, par.end, par.interval);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setSize(100, 25);
    
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
