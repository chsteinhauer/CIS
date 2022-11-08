
#pragma once
#include <JuceHeader.h>


class SimulationEditor : public juce::AudioProcessorEditor,
    private Timer,
    private Value::Listener
{
public:
    SimulationEditor(juce::AudioProcessor& owner);
    ~SimulationEditor() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    void hostMIDIControllerIsAvailable(bool controllerIsAvailable) override;
    int getControlParameterIndex(juce::Component& control) override;
    void updateTrackProperties();

private:
    juce::TextButton settings { "Settings" };
    juce::AudioProcessorValueTreeState::ButtonAttachment buttonAttachment;

    juce::Slider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment;


    // called when the stored window size changes
    void valueChanged(juce::Value&) override;
};