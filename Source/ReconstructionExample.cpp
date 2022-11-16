
#include "ReconstructionExample.h"


ReconstructionExample::ReconstructionExample() : osc([](float x) { return std::sin(x); }) { }
ReconstructionExample::~ReconstructionExample() { }

void ReconstructionExample::prepare(const juce::dsp::ProcessSpec& spec) {
	isPreparing = true;

	osc.prepare(spec);
	copy = new Block(memory, spec.numChannels, spec.maximumBlockSize);

	isPreparing = false;
}

void ReconstructionExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {

	bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();

	if (!sineEnabled && !noiseEnabled) return;

	copy->copyFrom(context.getOutputBlock());
	copy-> replaceWithAbsoluteValueOf(*copy);
	int N = copy->getNumChannels();
	
	for (int i = 0; i < N; i++)
	{
		auto freq = State::GetInstance()->getParameter("Fc")->convertFrom0to1(static_cast<float>(i+1) / (N+1));
		osc.setFrequency(freq);

		float* data = copy->getChannelPointer(i);

		for (int j = 0; j < copy->getNumSamples(); j++)
		{
			float sine = 0, noise = 0;

			if (sineEnabled)
				sine = osc.processSample(0);

			if (noiseEnabled)
				noise =  random.nextFloat();

			if (data != nullptr) {
				data[j] = data[j] * sine + data[j] * noise;
			}
		}
	}

	context.getOutputBlock().copyFrom(*copy);
}

void ReconstructionExample::reset() {
	osc.reset();
}
