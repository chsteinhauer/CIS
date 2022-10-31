
#include "PreprocessExample.h"

void PreprocessExample::prepare(const juce::dsp::ProcessSpec& spec) {
    iir.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate,440);

    iir.prepare(spec);
}

void PreprocessExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    iir.process(context);
}

void PreprocessExample::reset() {
    iir.reset();
}