
#include "ReconstructionExample.h"


ReconstructionExample::ReconstructionExample() : osc([](float x) { return std::sin(x); }) { }
ReconstructionExample::~ReconstructionExample() { }

void ReconstructionExample::prepare(const juce::dsp::ProcessSpec& spec) {
	osc.prepare(spec);
}

void ReconstructionExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {

	bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();

	if (!sineEnabled && !noiseEnabled) return;

	Block block(context.getOutputBlock());
	int N = block.getNumChannels();
	
	for (int i = 0; i < N; i++)
	{
		auto freq = State::GetInstance()->getParameter("Greenwood")->convertFrom0to1(static_cast<float>(i+1) / (N));
		osc.setFrequency(freq);

		float* data = block.getChannelPointer(i);

		for (int j = 0; j < block.getNumSamples(); j++)
		{
			float sine = 0, noise = 0;

			if (sineEnabled)
				sine = osc.processSample(0);

			if (noiseEnabled)
				noise = random.nextFloat()*2-1;

			if (data != nullptr) {
				data[j] = data[j] * sine + data[j];
			}
		}
	}
}

void ReconstructionExample::reset() {
	osc.reset();
}
