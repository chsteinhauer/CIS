


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
        auto no = new juce::AudioProcessorValueTreeState(processor, nullptr, "state", State::createParameters());
        pinstance_ = no;
    }
    return pinstance_;
}

juce::AudioProcessorValueTreeState* State::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return pinstance_;
}

juce::AudioProcessorValueTreeState::ParameterLayout State::createParameters() {

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // bools
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "ready",   1 }, "Ready", false));

    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "audio",   1 }, "Audio", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "sine",    1 }, "Sine", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "noise",   1 }, "Noise", false));

    // ranges
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "volume",    1 }, "Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "channelN",  1 }, "Number of Channels",
        juce::NormalisableRange<float>(0, 100, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Greenwood", 1 }, "Scaled Frequencies in Human Cochlear",
        getGreenwoodRange(greenwood(0), greenwood(1)), 20));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "fcenter", 1 }, "Center Frequency",
        juce::NormalisableRange<float>(450, 4500, 0.f, 1.f / std::log2(1.f + std::sqrt(4500 / 250))), 250));

    // Gain sliders
    for (int i = 1; i <= 100; i++)
        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "channel" + std::to_string(i), 1}, "Channel " + juce::String(i), 0.0f, 1.0f, 0.5f));

    return layout;
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

juce::NormalisableRange<float> State::getGreenwoodRange(float min, float max) 
{

    return juce::NormalisableRange<float> (min, max, 
        [=](float min, float, float v) { return greenwood(v); },
        [=](float min, float, float v) { return inverseGreenwood(v); }
    );
}

float State::inverseGreenwood(float x) {
    //Constants for greenwood function applied to the human cochlear
    /*const float A = 165.4f;
    const float a = 2.1f;
    const float K = 0.88f;*/

    //Adjusted constants for frequency range 250-4500Hz
    const float A = 34.1f;
    const float a = 2.1f;
    const float K = -6.35f;

    return (log10((x / A) + K) / a);
}

float State::greenwood(float x)
{
    ////Constants for greenwood function applied to the human cochlear
    //const float A = 165.4f;
    //const float a = 2.1f;
    //const float K = 0.88f;

    //Adjusted constants for frequency range 250-4500Hz
    const float A = 34.1f;
    const float a = 2.1f;
    const float K = -6.35f;

    if (x > 1 || x < 0)
    {
        return -1;
    }
    return A * (pow(10, (a * x)) - K);
}
