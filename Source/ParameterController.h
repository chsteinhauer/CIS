
#pragma once
#include <JuceHeader.h>
#include "SimulationState.h"


struct SettingsButton : juce::ImageButton {
    SettingsButton() {
        auto colour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(2.1F);

        juce::Image icon = juce::ImageCache::getFromMemory(BinaryData::settings24_png, BinaryData::settings24_pngSize);
        setImages(true, false, true, icon, 1.0, colour, icon, 1.0, colour.brighter(), icon, 1.0, colour);
    }

    void mouseEnter(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        juce::ImageButton::mouseEnter(evt);
    }

    void mouseExit(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::ImageButton::mouseExit(evt);
    }
};

struct VolumeSlider : juce::Slider {
    VolumeSlider() : attachment(*State::GetInstance(), "volume", *this) {
        auto par = State::GetInstance()->getParameterRange("volume");

        // default slider config
        setRange(par.start,par.end,par.interval);
        setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        setSize(100, 25);
    }

    void mouseDown(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        juce::Slider::mouseDown(evt);
    }

    void mouseUp(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::Slider::mouseUp(evt);
    }

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    juce::Image icon;
};

struct AudioToggleButton : juce::ImageButton, juce::AudioProcessorValueTreeState::Listener {

    AudioToggleButton() : attachment(*State::GetInstance(), "audio", *this) {
        audio = juce::ImageCache::getFromMemory(BinaryData::audio24_png, BinaryData::audio24_pngSize);
        mute = juce::ImageCache::getFromMemory(BinaryData::noaudio24_png, BinaryData::noaudio24_pngSize);

        setClickingTogglesState(true);
        setState(audio);

        onStateChange = [this] { toggleState(); };

        State::GetInstance() -> addParameterListener("volume", this);
    }

    void parameterChanged(const juce::String& parameterID, float newValue) override {
        if (parameterID == "volume" && getToggleState()) {
            setToggleState(false,juce::dontSendNotification);
            State::GetInstance()->getParameter("audio")->setValue(0);

            setState(audio);
        }
    }

    void setState(juce::Image state) {
        auto colour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(2.1F);

        setImages(true, false, true, state, 1.0, colour, state, 1.0, colour.brighter(), state, 1.0, colour);
    }

    void toggleState() {
        auto isMuted = getToggleState();

        setState(isMuted ? mute : audio);
    }

    void mouseEnter(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        juce::ImageButton::mouseEnter(evt);
    }

    void mouseExit(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::ImageButton::mouseExit(evt);
    }

    juce::Image audio;
    juce::Image mute;

    juce::AudioProcessorValueTreeState::ButtonAttachment attachment;
};