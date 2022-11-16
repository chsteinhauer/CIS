
#include "AnalyseExample.h"

void AnalyseExample::prepare(const juce::dsp::ProcessSpec& spec) {
    /*filters.reset();
    filters.prepare(spec);

    for (int i = 0; i < spec.numChannels;i++) {
        prepareFilters(spec, i);
    }*/
}

void AnalyseExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    //filters.process(context);
}

void AnalyseExample::reset() {
    //filters.reset();
}
