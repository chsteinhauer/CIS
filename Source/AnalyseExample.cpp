
#include "AnalyseExample.h"

void AnalyseExample::prepare(const juce::dsp::ProcessSpec& spec) {
   /* for (int i = 0; i < filters.size(); ++i) {
        filters[i].reset();
        *filters[i].state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, frequencies[i], filterQ[i]);
        filters[i].prepare(spec);
    }*/
}

void AnalyseExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
  /*  for (int i = 0; i < filters.size(); ++i) {
        filters[i].process(context);
    }*/
}

void AnalyseExample::reset() {
    /*for (int i = 0; i < filters.size(); ++i) {
        filters[i].reset();
    }*/
}