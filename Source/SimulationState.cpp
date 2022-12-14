


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

        std::srand(1);
        for (int i = 0; i < maxNumChannels; i++) {
            // generate random colours
            colours[i] = juce::Colour(
                (double)std::rand() / RAND_MAX * 255,
                (double)std::rand() / RAND_MAX * 255,
                (double)std::rand() / RAND_MAX * 255
            );
        }
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
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "audio",   1 }, "Audio", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "sine",    1 }, "Sine", true));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "noise",   1 }, "Noise", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "pshc",   1 }, "PSHC", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "togglemedia",   1 }, "togglemedia", false));

    // ranges
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "fmin",    1 }, "Frequency min",
        juce::NormalisableRange<float>(20, 20000, 0.1f), 250));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "fmax",    1 }, "Frequency max",
        juce::NormalisableRange<float>(20, 20000, 0.1f), 4500));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "volume",    1 }, "Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "sinegain",    1 }, "Sine Gain",
        juce::NormalisableRange<float>(0.0f, 2.0f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "noisegain",    1 }, "Noise Gain",
        juce::NormalisableRange<float>(0.0f, 2.0f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "pshcgain",    1 }, "PSHC Gain",
        juce::NormalisableRange<float>(0.0f, 2.0f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "threshold",    1 }, "Compressor Threshold",
        juce::NormalisableRange<float>(-100.0f, 0.0f), -40.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "makeupgain",    1 }, "Make-up Gain",
        juce::NormalisableRange<float>(0.0f, 100.0f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "dynamicrange",    1 }, "Dynamic Range",
        juce::NormalisableRange<float>(0.0f, 100.0f), 60.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "channelN",  1 }, "Number of Channels",
        juce::NormalisableRange<float>(0, maxNumChannels, 1), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Greenwood", 1 }, "Scaled Frequencies in Human Cochlear",
        getGreenwoodRange(20, 20000), 20));

    // Gain sliders
    for (int i = 1; i <= maxNumChannels; i++) {
        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "channel" + std::to_string(i), 1 }, "Channel " + juce::String(i), 0.0f, 2.0f, 1.0f));
    }

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

juce::NormalisableRange<float> State::getGreenwood()
{
    auto min = State::GetDenormalizedValue("fmin");
    auto max = State::GetDenormalizedValue("fmax");

    // TODO: small hack, we need a better way to handle this
    if (min == max)
        max = max + 1;

    return juce::NormalisableRange<float>(min, max,
        [=](float min, float, float v) { return greenwood(v); },
        [=](float min, float, float v) { return inverseGreenwood(v); }
    );
}

juce::NormalisableRange<float> State::getGreenwoodRange(float min, float max) 
{
    return juce::NormalisableRange<float> (min, max, 
        [=](float min, float, float v) { return greenwood(v); },
        [=](float min, float, float v) { return inverseGreenwood(v); }
    );
}

float State::inverseGreenwood(float f) {
    //Constants for greenwood function applied to the human cochlear
    const float A = 165.4f;
    const float a = 2.1f;
    const float K = 0.88f;

    return (log10((f / A) + K) / a);
}

float State::greenwood(float x)
{
    //Constants for greenwood function applied to the human cochlear
    const float A = 165.4f;
    const float a = 2.1f;
    const float K = 0.88f;

    if (pinstance_ != nullptr) {
        const float min = 0.0f;
        const float max = 1.0f;
        auto newMin = inverseGreenwood(State::GetDenormalizedValue("fmin"));
        auto newMax = inverseGreenwood(State::GetDenormalizedValue("fmax"));

        x = map(x, min, max, newMin, newMax);
    }

    if (x > 1 || x < 0)
    {
        return -1;
    }
    return A * (pow(10, (a * x)) - K);
}

float State::map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
