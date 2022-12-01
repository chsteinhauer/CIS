
#include "PreprocessExample.h"
#include "SimulationState.h"

void PreprocessExample::prepare(const juce::dsp::ProcessSpec& spec) {
    static const int fmin = State::GetDenormalizedValue("fmin");
    static const int fmax = State::GetDenormalizedValue("fmax");

    float fcenter = fmin * pow(static_cast<float>(fmax) / fmin, 0.5);
    float filterQ = sqrt(fmin * fmax) / (fmax - fmin);

    iir.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, fcenter, filterQ);
    iir.prepare(spec);
}

void PreprocessExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    iir.process(context);
}

void PreprocessExample::reset() {
    iir.reset();
}