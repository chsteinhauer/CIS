
#include "ReconstructionExample.h"

void ReconstructionExample::prepare(const juce::dsp::ProcessSpec& spec) {
	int N = State::GetDenormalizedValue("channelN");
	
	oscillators.clear();

	for (int i = 0; i < N; i++)
	{
		juce::dsp::Oscillator<float>* osc = setupSinewave(i, N);
		osc->prepare(spec);
		oscillators.push_back(osc);
	}

	tempBlock = juce::dsp::AudioBlock<float>(tempBlockMemory, N, spec.maximumBlockSize);
	noiseBlock = juce::dsp::AudioBlock<float>(noiseBlockMemory, N, spec.maximumBlockSize);
}

void ReconstructionExample::process(const juce::dsp::ProcessContextReplacing< float >& context) {
	tempBlock.copyFrom(context.getOutputBlock());

	for (int i = 0; i < tempBlock.getNumChannels(); i++)
	{
		bool sineEnabled = State::GetInstance()->getParameter("sine")->getValue();
		if (sineEnabled) {
			juce::dsp::AudioBlock<float>& envelope(tempBlock.getSingleChannelBlock(i));
			juce::dsp::ProcessContextReplacing<float> tmpContext(envelope);

			auto osc = oscillators.at(i);
			osc->process(tmpContext);

			context.getOutputBlock().getSingleChannelBlock(i).add(tmpContext.getOutputBlock());
		}
	}

	bool noiseEnabled = State::GetInstance()->getParameter("noise")->getValue();
	if (noiseEnabled) {
		juce::dsp::AudioBlock<float>& envelope(tempBlock);
		juce::dsp::ProcessContextReplacing<float> tmpContext(envelope);
		generateNoise();

		tmpContext.getOutputBlock().multiplyBy(noiseBlock);
		context.getOutputBlock().add(tmpContext.getOutputBlock());
	}
}

void ReconstructionExample::reset() {

	for (int i = 0; i < tempBlock.getNumChannels(); i++) {
	
		oscillators.at(i)->reset();
	}
}

juce::dsp::Oscillator<float>* ReconstructionExample::setupSinewave(int index, int N) {
	auto Fc = State::GetInstance()->getParameter("Fc")->convertFrom0to1(((float)index/N));
	auto osc = new juce::dsp::Oscillator<float>(); 

	osc->initialise([this](float x) { return std::sin(x); }, 128);
	osc->setFrequency(Fc);

	return osc;
}

void ReconstructionExample::generateNoise() {
	for (int i = 0; i < noiseBlock.getNumSamples(); i++)
	{
		for (int j = 0; j < noiseBlock.getNumChannels(); j++)
		{
			auto value = random.nextFloat() * 0.25f - 0.125f;

			noiseBlock.setSample(j,i,value);
		}
	}
}