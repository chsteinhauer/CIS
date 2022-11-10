#pragma once
#include <JuceHeader.h>

class State
{

private:
    static juce::AudioProcessorValueTreeState* pinstance_;
    static std::mutex mutex_;

protected:
    State();
    ~State();

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

public:
    State(State& other) = delete;
    void operator=(const State&) = delete;

    static juce::AudioProcessorValueTreeState *Initialize(juce::AudioProcessor& processor);
    static juce::AudioProcessorValueTreeState *GetInstance();
};

