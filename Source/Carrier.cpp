
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
    constexpr auto WAVETABLE_LENGTH = 64;
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
    return rand(rng);
}


PSHC::PSHC() { }
PSHC::~PSHC() { }

void PSHC::prepare(const juce::dsp::ProcessSpec& spec) {
    int N = spec.numChannels;
    if (N <= 0) return;

    std::vector<HarmonicComplex> _pshcs(N);

    auto gw = State::GetInstance()->getParameter("Greenwood");

    float lo = gw->convertFrom0to1(0);
    float hi = 0;

    for (auto i = 0; i < N; ++i)
    {
        hi = gw->convertFrom0to1(static_cast<float>(i + 1) / N);
        
        _pshcs.at(i).genHarmonics(lo, hi, spec.sampleRate);

        lo = hi;
    }

    pshcs.swap(_pshcs);
    _pshcs.clear();
}

float PSHC::next(int channel) {
    return pshcs.at(channel).getSample();
}

PSHC::HarmonicComplex::HarmonicComplex() {}

void PSHC::HarmonicComplex::genHarmonics(float lo, float hi, float sampleRate) {
    float fcenter = lo * pow(static_cast<float>(hi) / lo, 0.5);

    const float M = State::GetDenormalizedValue("fmin");
    const float N = State::GetDenormalizedValue("fmax");
    const float PI = 2 * std::atanf(1.f) * 4;

    // Calculate pulse-per-second (pps) based on formula from Mesnildrey et al. (2016)
    const float fkHz = fcenter * 0.001;
    const float pps = 37 + 151 * fkHz + 0.17 * pow(fkHz, 2);

    // The square root of pps determines the order k
    const int k = std::roundf(std::sqrtf(pps));
    const int TABLE_LENGTH = (sampleRate / ( k * k));

    std::vector<int> r(k);
    std::iota(std::begin(r), std::end(r), 0);
    std::shuffle(std::begin(r), std::end(r), rng);

    std::vector<int> u(k);
    u = r;
    std::shuffle(std::begin(u), std::end(u), rng);

    std::vector<float> harmonics = std::vector<float>(TABLE_LENGTH, 0.f);

    for (int i = lo; i < hi; ++i) {

        auto j = i % k;
        auto phase = PI * ((static_cast<float>(i) / pow(k, 2)) * r.at(j) + (u.at(j) % 2));
        auto delta = PI * f0 * i * (1/sampleRate);
        auto angle = phase;

        for (int l = 0; l < TABLE_LENGTH; ++l) {
            harmonics.at(l) += std::sinf(angle);

            angle += delta;
        }
    }

    gammatone::filter<float> gammatoneFilter(sampleRate, fcenter);
    gammatoneFilter.compute_range(harmonics.begin(), harmonics.end(), harmonics.begin());

    auto max = *std::max_element(std::begin(harmonics), std::end(harmonics)); 
    auto min = *std::min_element(std::begin(harmonics), std::end(harmonics));

    auto fdiff = 1 / (max - min);
    
    for (auto it = harmonics.begin(); it != harmonics.end(); it++) {
        *it = 2 * (*it - min) * fdiff - 1;
    }
    
    pshc.swap(harmonics);
    harmonics.clear();
    r.clear();
}

float PSHC::HarmonicComplex::getSample() {
    if (currentIndex == pshc.size())
        currentIndex = 0;

    return pshc.at(currentIndex++);
}

void PSHC::HarmonicComplex::reset() {
    pshc.clear();
}

void PSHC::reset() {
    for (auto& pshc : pshcs)
        pshc.reset();

    pshcs.clear();
}
