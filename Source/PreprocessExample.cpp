
#include "PreprocessExample.h"

void PreprocessExample::prepare(const juce::dsp::ProcessSpec& spec) {
     static const int fmin = 250;
     static const int fmax = 4500;

     float fcenter = static_cast<float>((fmax - fmin)) / 2;

     float filterQ = sqrt(fmin * fmax) / (fmax - fmin); //sqrt(freqstep) / (freqstep - 1);

    iir.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, fcenter, filterQ);
    iir.prepare(spec);
}

void PreprocessExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    iir.process(context);
}

void PreprocessExample::reset() {
    iir.reset();
}