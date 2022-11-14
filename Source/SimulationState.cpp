


#include "SimulationState.h"


State::State() {}
State::~State() {}

/**
 * Static methods should be defined outside the class.
 */

juce::AudioProcessorValueTreeState* State::pinstance_{ nullptr };
std::mutex State::mutex_;

//std::vector <cb_t> State::callbackFunctions;

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
        auto no = new juce::AudioProcessorValueTreeState(processor, nullptr, "state", State::createParameters());
        pinstance_ = no;

        //State::runCallbacks();
    }
    return pinstance_;
}

juce::AudioProcessorValueTreeState* State::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);

    return pinstance_;
}

juce::AudioProcessorValueTreeState::ParameterLayout State::createParameters() {
    return {
        // bools
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "audio",   1 }, "Audio",     false),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "sine",    1 }, "Sine",      false),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "noise",   1 }, "Noise",     false),

        // ranges
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "volume",  1 }, "Volume",
            juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "channelN",1 }, "Number of Channels",  
            juce::NormalisableRange<float>(0, 24), 6),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "Fc",1 },       "Center Frequencies",        
            juce::NormalisableRange<float>(250, 4500, 0.f, 0.25f), 250),
    };
}

int State::GetDenormalizedValue(std::string id) {
    auto par = State::GetInstance()->getParameter(id);

    return par->convertFrom0to1(par ->getValue());
}
