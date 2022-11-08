


#include "SimulationState.h"

State::State() {}
State::~State() {}

/**
 * Static methods should be defined outside the class.
 */

juce::AudioProcessorValueTreeState* State::pinstance_{ nullptr };
std::mutex State::mutex_;

/**
 * The first time we call State we will lock the storage location
 * and then we make sure again that the variable is null and then we
 * set the value.
 */
juce::AudioProcessorValueTreeState* State::Initialize(juce::AudioProcessor& processor)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new juce::AudioProcessorValueTreeState(processor, nullptr, "state", State::createParameters());
    }
    return pinstance_;
}

juce::AudioProcessorValueTreeState* State::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        return pinstance_;
    }
    return pinstance_;
}

juce::AudioProcessorValueTreeState::ParameterLayout State::createParameters() {
    return {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "volume",  1 }, "Volume",     juce::NormalisableRange<float>(0.0f, 1.0f), 0.7f),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "settings",1 }, "Settings",   false),
    };
}