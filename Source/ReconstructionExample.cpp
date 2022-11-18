
#include "ReconstructionExample.h"


ReconstructionExample::ReconstructionExample() { }
ReconstructionExample::~ReconstructionExample() { }

void ReconstructionExample::prepare(const juce::dsp::ProcessSpec& spec) {
	synth.prepare(spec);
	butterworth.remakeFilters(spec.numChannels,spec.sampleRate);
}

void ReconstructionExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
	synth.process(context);
	butterworth.process(context.getOutputBlock());
}

void ReconstructionExample::reset() {
	synth.reset();
	butterworth.clearFilters();
}


ReconstructionExample::Synthesis::Synthesis() : osc([](float x) { return std::sin(x); }) {}
ReconstructionExample::Synthesis::~Synthesis() {}

void ReconstructionExample::Synthesis::prepare(const juce::dsp::ProcessSpec& spec) {
	osc.prepare(spec);
}

void ReconstructionExample::Synthesis::process(const juce::dsp::ProcessContextReplacing<float>& context) {

	bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();

	if (!sineEnabled && !noiseEnabled) return;

	Block block(context.getOutputBlock());
	int N = block.getNumChannels();

	auto gw = State::GetInstance()->getParameter("Greenwood");

	for (int i = 0; i < N; i++)
	{
		auto fcenter = gw->convertFrom0to1((i + static_cast<float>(1)) / N);
		osc.setFrequency(fcenter);

		auto par = State::GetInstance()->getParameter("channel" + std::to_string(i + 1));
		auto value = par->getValue();
		gain.setGainLinear(value);

		float* data = block.getChannelPointer(i);

		for (int j = 0; j < block.getNumSamples(); j++)
		{
			float sine = 0, noise = 0;

			if (sineEnabled)
				sine = osc.processSample(0);

			if (noiseEnabled)
				noise = random.nextFloat();

			if (data != nullptr) {
				data[j] = data[j] * sine + data[j] * noise;
			}
		}

		gain.process(juce::dsp::ProcessContextReplacing<float>(block.getSingleChannelBlock(i)));
	}
}

void ReconstructionExample::Synthesis::reset() {
	osc.reset();
}
