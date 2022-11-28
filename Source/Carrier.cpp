
#include "Carrier.h"


Sine::Sine() { }
Sine::~Sine() { }

void Sine::prepare(const juce::dsp::ProcessSpec& spec) {
    auto gw = State::GetInstance()->getParameter("Greenwood");

    float lo = gw->convertFrom0to1(0);
    float hi = 0;

    int N = spec.numChannels;

    oscillators.clear(); 

    for (auto i = 0; i < N; ++i)  
    {
        hi = gw->convertFrom0to1(static_cast<float>(i + 1) / N);
        float fcenter = lo * pow(static_cast<float>(hi) / lo, 0.5);

        oscillators.emplace_back(sineWaveTable, spec.sampleRate); 
        oscillators.at(i).setFrequency(fcenter);

        lo = hi;
    }
}

float Sine::next(int channel) {
    return oscillators.at(channel).getSample();
}

void Sine::reset() {
    oscillators.clear();
}

std::vector<float> Sine::generateSineWaveTable()
{
    constexpr auto WAVETABLE_LENGTH = 128;
    const auto PI = std::atanf(1.f) * 4;
    std::vector<float> sineWaveTable = std::vector<float>(WAVETABLE_LENGTH);

    for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
    {
        sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / WAVETABLE_LENGTH);
    }

    return sineWaveTable;
}

Sine::WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate)
    : waveTable{ std::move(waveTable) },
    sampleRate{ sampleRate }
{}

float Sine::WavetableOscillator::getSample()
{
    index = std::fmod(index, static_cast<float>(waveTable.size()));
    const auto sample = interpolateLinearly();
    index += indexIncrement;
    return sample;
}

void Sine::WavetableOscillator::setFrequency(float frequency)
{
    indexIncrement = frequency * static_cast<float>(waveTable.size())
        / static_cast<float>(sampleRate);
}

float Sine::WavetableOscillator::interpolateLinearly() const
{
    const auto truncatedIndex = static_cast<typename  decltype(waveTable)::size_type>(index);
    const auto nextIndex = static_cast<typename  decltype(waveTable)::size_type>
        (std::ceil(index)) % waveTable.size();
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    return waveTable[nextIndex] * nextIndexWeight +
        (1.f - nextIndexWeight) * waveTable[truncatedIndex];
}

Noise::Noise() { }
Noise::~Noise() { }

float Noise::next() {
    return random.nextFloat();
}


PSHC::PSHC() { }
PSHC::~PSHC() { }

void PSHC::prepare(const juce::dsp::ProcessSpec& spec) {
    std::vector<int> tmp(7);
    std::iota(std::begin(tmp), std::end(tmp), 0);

    r.clear();
    r.swap(tmp);

    tmp.clear();

    std::shuffle(std::begin(r), std::end(r), gen);
}

void PSHC::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    int k = 10;
    
}

float PSHC::genHarmonic(int k) {
    auto& r_ = r;

    float M = State::GetDenormalizedValue("fmin");
    float N = State::GetDenormalizedValue("fmax");

    float sine = 0.f;

   /* for (int i = M; i < N; i++) {
        auto j = (i % k) + 1;
        auto p = juce::MathConstants<float>::twoPi * ((i / pow(k, 2)) * r_.back());
        sine += std::sinf();

        r_.pop_back();
    }*/

    return sine;
}

void PSHC::reset() {
}
