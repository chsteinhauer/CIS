
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
	sampleRate = spec.sampleRate;
}

void ReconstructionExample::Synthesis::process(const juce::dsp::ProcessContextReplacing<float>& context) {

	bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();

	if (!sineEnabled && !noiseEnabled) return;

	Block block(context.getOutputBlock());
	int N = block.getNumChannels();

	auto gw = State::GetInstance()->getParameter("Greenwood");

	float lo = gw->convertFrom0to1(0);
	float hi = 0;
	for (int i = 1; i <= N; i++)
	{
		int channel = i - 1;
		hi = gw->convertFrom0to1(static_cast<float>(i) / N);

		// setup sinewave
		float fcenter = lo * pow(static_cast<float>(hi) / lo, 0.5);
		auto cyclesPerSample = fcenter / sampleRate;
		float delta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
		float angle = 0;

		float* data = block.getChannelPointer(channel);

		for (int j = 0; j < block.getNumSamples(); j++)
		{
			float sine = 0, noise = 0;

			if (sineEnabled) {
				sine = sin(angle);
				angle += delta;
			}

			if (noiseEnabled)
				noise = random.nextFloat();

			if (data != nullptr) {
				data[j] = (data[j] * sine + data[j] * noise);
			}
		}

		auto par = State::GetInstance()->getParameter("channel" + std::to_string(i));
		auto value = par->getValue();
		gain.setGainLinear(value);

		gain.process(juce::dsp::ProcessContextReplacing<float>(block.getSingleChannelBlock(channel)));

		lo = hi;
	}
}

void ReconstructionExample::Synthesis::reset() { }
