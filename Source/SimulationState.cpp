


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
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "ready",   1 }, "Ready",     false),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "audio",   1 }, "Audio",     false),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "sine",    1 }, "Sine",      false),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID  { "noise",   1 }, "Noise",     false),

        // ranges
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "volume",    1 }, "Volume",
            juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "channelN",  1 }, "Number of Channels",  
            juce::NormalisableRange<float>(0, 120, 1), 6),
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "Greenwood", 1 }, "Scaled Frequencies in Human Cochlear",        
            getGreenwoodRange(), 20),
    };
}

float State::GetDenormalizedValue(std::string id) {
    auto par = State::GetInstance()->getParameter(id);

    return par->convertFrom0to1(par ->getValue());
}

juce::StringArray State::GetAllValueStrings(std::string id) {
    auto par = State::GetInstance()->getParameterRange(id);

    juce::StringArray values = juce::StringArray();

    for (int val = par.start; val <= par.end; val += par.interval) {
        values.add(juce::String(val));
    }

    return values;
}

juce::NormalisableRange<float> State::getGreenwoodRange() 
{
    float min = 20;
    float max = greenwood(1);

    return juce::NormalisableRange<float> (min, max, 
        [=](float min, float, float v) { return greenwood(v); },
        [=](float min, float, float v) { return inverseGreenwood(v); }
    );
}

float State::inverseGreenwood(float x) {
    const float A = 165.4f;
    const float a = 2.1f;
    const float K = 0.88f;

    return (log10((x / A) + K) / a);
}

float State::greenwood(float x)
{
    //Constants for greenwood function applied to the human cochlear
    const float A = 165.4f;
    const float a = 2.1f;
    const float K = 0.88f;

    if (x > 1 || x < 0)
    {
        return -1;
    }
    return A * (pow(10, (a * x)) - K);
}
