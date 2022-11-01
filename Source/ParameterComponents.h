/*
  ==============================================================================

    ParameterComponents.h
    Created: 1 Nov 2022 11:46:53am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct ParameterBase : public juce::ChangeBroadcaster
{
    ParameterBase(const juce::String& labelName) : name(labelName) {}
    virtual ~ParameterBase() {}

    virtual juce::Component* getComponent() = 0;

    virtual int getPreferredHeight() = 0;
    virtual int getPreferredWidth() = 0;

    juce::String name;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterBase)
};

//==============================================================================
struct SliderParameter : public ParameterBase
{
    SliderParameter(juce::Range<double> range, double skew, double initialValue,
        const juce::String& labelName, const juce::String& suffix = {})
        : ParameterBase(labelName)
    {
        slider.setRange(range.getStart(), range.getEnd(), 0.01);
        slider.setSkewFactor(skew);
        slider.setValue(initialValue);

        if (suffix.isNotEmpty())
            slider.setTextValueSuffix(suffix);

        slider.onValueChange = [this] { sendChangeMessage(); };
    }

    juce::Component* getComponent() override { return &slider; }

    int getPreferredHeight() override { return 40; }
    int getPreferredWidth()  override { return 500; }

    double getCurrentValue() const { return slider.getValue(); }

private:
    juce::Slider slider;
};

//==============================================================================
struct ChoiceParameter : public ParameterBase
{
    ChoiceParameter(const juce::StringArray& options, int initialId, const juce::String& labelName)
        : ParameterBase(labelName)
    {
        parameterBox.addItemList(options, 1);
        parameterBox.onChange = [this] { sendChangeMessage(); };

        parameterBox.setSelectedId(initialId);
    }

    juce::Component* getComponent() override { return &parameterBox; }

    int getPreferredHeight() override { return 25; }
    int getPreferredWidth()  override { return 250; }

    int getCurrentSelectedID() const { return parameterBox.getSelectedId(); }

private:
    juce::ComboBox parameterBox;
};
