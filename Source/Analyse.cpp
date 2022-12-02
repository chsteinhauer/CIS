
#include "Analyse.h"

void Analyse::prepare(const juce::dsp::ProcessSpec& spec) {
    bandpass.remakeFilters(spec);
    extractor.prepareHalfwaveRectification(spec);
}

void Analyse::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    bandpass.process(context.getOutputBlock());
    extractor.halfwaveRectification(context.getOutputBlock());
}

void Analyse::reset() {
    bandpass.clearFilters();
}
