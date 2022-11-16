
#include "AnalyseExample.h"

void AnalyseExample::prepare(const juce::dsp::ProcessSpec& spec) {
    bandpass.remakeFilters(spec.numChannels,spec.sampleRate);
    extractor.prepareHalfwaveRectification(spec);
}

void AnalyseExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    bandpass.process(context.getOutputBlock());
    extractor.halfwaveRectification(context.getOutputBlock());
}

void AnalyseExample::reset() {
    bandpass.clearFilters();
}
