
#include "ReconstructionExample.h"


ReconstructionExample::ReconstructionExample() { }
ReconstructionExample::~ReconstructionExample() { }

void ReconstructionExample::prepare(const juce::dsp::ProcessSpec& spec) {
	synth.prepare(spec);
	butterworth.remakeFilters(spec);
}

void ReconstructionExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
	synth.process(context);
	butterworth.process(context.getOutputBlock());
}

void ReconstructionExample::reset() {
	synth.reset();
	butterworth.clearFilters();
}


ReconstructionExample::Synthesis::Synthesis() {}
ReconstructionExample::Synthesis::~Synthesis() {}

void ReconstructionExample::Synthesis::prepare(const juce::dsp::ProcessSpec& spec) {
	sine.prepare(spec);
}

void ReconstructionExample::Synthesis::process(const juce::dsp::ProcessContextReplacing<float>& context) {

	bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();

	if (!sineEnabled && !noiseEnabled) return;

	Block block(context.getOutputBlock());

	for (int channel = 0; channel < block.getNumChannels(); channel++)
	{
		float* data = block.getChannelPointer(channel);

		for (int i = 0; i < block.getNumSamples(); i++)
		{
			// carriers
			float csine = 0, cnoise = 0;

			if (sineEnabled) {
				csine = sine.next(channel);
			}

			if (noiseEnabled)
				cnoise = noise.next();

			if (data != nullptr) {
				data[i] = data[i] * csine + data[i] * cnoise;
			}
		}

		auto par = State::GetInstance()->getParameter("channel" + std::to_string(channel+1));
		auto value = par->getValue();
		gain.setGainLinear(value);

		gain.process(juce::dsp::ProcessContextReplacing<float>(block.getSingleChannelBlock(channel)));
	}
}

void ReconstructionExample::Synthesis::reset() { 
	sine.reset();
}
