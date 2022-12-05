
#include "Analyse.h"

void Analyse::prepare(const juce::dsp::ProcessSpec& spec) {
    if (spec.numChannels <= 0) {
        return;
    }

    bandpass.remakeFilters(spec);
    extractor.prepareHalfwaveRectification(spec);
}

void Analyse::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    if (context.isBypassed) {
        return;
    }
    bandpass.process(context.getOutputBlock());
    extractor.halfwaveRectification(context.getOutputBlock());
}

void Analyse::reset() {
    bandpass.clearFilters();
}
