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

public:
    State(State& other) = delete;
    void operator=(const State&) = delete;

    static juce::AudioProcessorValueTreeState *Initialize(juce::AudioProcessor& processor);
    static juce::AudioProcessorValueTreeState *GetInstance();
    static int GetDenormalizedValue(std::string id);
};

