
#include "Preprocess.h"
#include "SimulationState.h"

void Preprocess::prepare(const juce::dsp::ProcessSpec& spec) {
    if (spec.numChannels <= 0 ) {
        return;
    }

    static const int fmin = State::GetDenormalizedValue("fmin");
    static const int fmax = State::GetDenormalizedValue("fmax");

    float fcenter = fmin * pow(static_cast<float>(fmax) / fmin, 0.5);
    float filterQ = sqrt(fmin * fmax) / (fmax - fmin);

    iir.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, fcenter, filterQ);
    iir.prepare(spec);

    compressor.setThreshold(-20);
    compressor.setRatio(12);
    compressor.setAttack(5);
    compressor.setRelease(100);
    compressor.prepare(spec);

    expander.setThreshold(-40);
    expander.setRatio(12);
    expander.setAttack(5);
    expander.setRelease(100);
    expander.prepare(spec);
}

void Preprocess::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    if (context.isBypassed) {
        return;
    }
    iir.process(context);

    // Do not uncomment
    //compressor.process(context);
    //expander.process(context);
}

void Preprocess::reset() {
    iir.reset();
    compressor.reset();
    expander.reset();
}
