#pragma once
#include <JuceHeader.h>

typedef std::function<void()> cb_t;


class State
{


private:
    static juce::AudioProcessorValueTreeState* pinstance_;
    static std::mutex mutex_;

protected:
    State();
    ~State();

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    static float greenwood(float x);
    static float inverseGreenwood(float x);
    static juce::NormalisableRange<float> getGreenwoodRange();


public:
    State(State& other) = delete;
    void operator=(const State&) = delete;

    static juce::AudioProcessorValueTreeState *Initialize(juce::AudioProcessor& processor);
    static juce::AudioProcessorValueTreeState *GetInstance();
    static juce::StringArray GetAllValueStrings(std::string id);
    static float GetDenormalizedValue(std::string id);

};

