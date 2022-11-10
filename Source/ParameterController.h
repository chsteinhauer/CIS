
#pragma once
#include <JuceHeader.h>
#include "SimulationState.h"


struct ControllerBase
{
    ControllerBase(std::string id, std::string labelText) 
        : label(id,labelText) {
    }

    juce::Label label;
};

struct IconController : ControllerBase, juce::ImageButton {

    IconController(std::string id, std::string labelText, const void* imageData, int size) 
        : ControllerBase(id,labelText), attachment(*State::GetInstance(), id, *this) {

        auto colour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(2.1F);

        juce::Image icon = juce::ImageCache::getFromMemory(imageData, size);
        setImages(true, false, true, icon, 1.0, colour, icon, 1.0, colour.brighter(), icon, 1.0, colour);
    
        label.attachToComponent(this,false);
        addAndMakeVisible(label);

    }

    void mouseEnter(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        juce::ImageButton::mouseEnter(evt);
    }

    void mouseExit(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::ImageButton::mouseExit(evt);
    }

    juce::AudioProcessorValueTreeState::ButtonAttachment attachment;
};

struct SliderController : ControllerBase, juce::Slider {

    SliderController(std::string id, std::string labelText, double max, double min, double interval = (0.0))
        : ControllerBase(id, labelText), attachment(*State::GetInstance(), id, *this) {

        // default slider config
        setRange(max,min,interval);
        setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        setSize(100, 25);

        //auto colour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter(2.1F);

     /*   juce::Image icon = juce::ImageCache::getFromMemory(BinaryData::settings24_png, BinaryData::settings24_pngSize);
        auto cmp = new juce::ImageComponent();
        cmp->setImage(icon);
        cmp->setSize(25,25);*/
        
   /*     label.setText("",juce::dontSendNotification);
        label.addAndMakeVisible(cmp);
        label.attachToComponent(this,false);*/
        //addAndMakeVisible(label);
    }

    void mouseEnter(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        juce::Slider::mouseEnter(evt);
    }

    void mouseDown(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        juce::Slider::mouseDown(evt);
    }

    void mouseUp(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::Slider::mouseUp(evt);
    }

    void mouseExit(const juce::MouseEvent& evt) override {
        setMouseCursor(juce::MouseCursor::NormalCursor);
        juce::Slider::mouseExit(evt);
    }

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;
    juce::Image icon;
};