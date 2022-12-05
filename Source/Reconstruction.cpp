
#include "Reconstruction.h"

Reconstruction::Reconstruction() { }
Reconstruction::~Reconstruction() { }

void Reconstruction::prepare(const juce::dsp::ProcessSpec& spec) {
	if (spec.numChannels <= 0) {
		return;
	}

	synth.prepare(spec);
	butterworth.remakeFilters(spec);
	compressor.reset();
	expander.reset();

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

void Reconstruction::process(const juce::dsp::ProcessContextReplacing<float>& context) {
	if (context.isBypassed) {
		return;
	}
	synth.process(context);
	auto test = context.getOutputBlock();
	butterworth.process(context.getOutputBlock());
	compressor.process(context);
	auto test2 = context.getOutputBlock();
	expander.process(context);
	auto test3 = context.getOutputBlock();
}

void Reconstruction::reset() {
	synth.reset();
	butterworth.clearFilters();
	compressor.reset();
	expander.reset();
}

Reconstruction::Synthesis::Synthesis() {}
Reconstruction::Synthesis::~Synthesis() {}

void Reconstruction::Synthesis::prepare(const juce::dsp::ProcessSpec& spec) {
	sine.prepare(spec);
	pshc.prepare(spec);
}

void Reconstruction::Synthesis::process(const juce::dsp::ProcessContextReplacing<float>& context) {

	bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();
	bool pshcEnabled = State::GetInstance()->getParameter("pshc")->getValue();

	if (!sineEnabled && !noiseEnabled && !pshcEnabled) return;

	Block block(context.getOutputBlock());

	for (int channel = 0; channel < block.getNumChannels(); channel++)
	{

		float* data = block.getChannelPointer(channel);

		for (int i = 0; i < block.getNumSamples(); i++)
		{
			// carriers
			float csine = 0, cnoise = 0, cpshc = 0;

			if (sineEnabled) 
				csine = sine.next(channel);

			if (noiseEnabled)
				cnoise = noise.next();

			if (pshcEnabled)
				cpshc = pshc.next(channel);

			if (data != nullptr) {
				data[i] = data[i] * csine + data[i] * cnoise + data[i] * cpshc;
			}
		}

		// Set gain for a single channel
		auto par = State::GetInstance()->getParameter("channel" + std::to_string(channel + 1));
		auto value = par->getValue();
		
		gain.setGainLinear(value);
		gain.process(juce::dsp::ProcessContextReplacing<float>(block.getSingleChannelBlock(channel)));
	}
}

void Reconstruction::Synthesis::reset() { 
	sine.reset();
	pshc.reset();
}
