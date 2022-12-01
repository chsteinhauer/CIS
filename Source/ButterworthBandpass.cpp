
#include "ButterworthBandpass.h"
#include "SimulationState.h"

ButterworthBandpass::ButterworthBandpass() { }
ButterworthBandpass::~ButterworthBandpass() { }

void ButterworthBandpass::remakeFilters(const juce::dsp::ProcessSpec& spec)
{
	int N = spec.numChannels;
	std::vector<ButterworthSixthOrder> tmp(N);

	auto gw = State::GetInstance()->getParameter("Greenwood");

	bool randomizeOrder = State::GetInstance()->getParameterAsValue("RandomOrder").getValue();

	float lowFreq = gw->convertFrom0to1(0); 
	int order;
	float highFreq;
	for (int i = 1; i <= N; i++)
	{
		if (randomizeOrder)
			order = 5 + random.nextFloat() * 2; // Random value between 5 & 7
		else
			order = 6;

		highFreq = gw->convertFrom0to1(((float)i / N));
		tmp.at((size_t)i-1).prepare(lowFreq, highFreq, { spec.sampleRate, (juce::uint32)spec.maximumBlockSize, (juce::uint32)1 }, order);

		lowFreq = highFreq;
	}

	bandpasses.swap(tmp);

	tmp.clear();
}

void ButterworthBandpass::clearFilters()
{
	for (int i = 0; i < bandpasses.size(); i++) {
		bandpasses.at(i).reset();
	}
	bandpasses.clear();
}

void ButterworthBandpass::process(juce::dsp::AudioBlock<float> block)
{
	int N = block.getNumChannels();

	for (int i = 0; i < N; i++) {
		juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));

		auto& test = bandpasses.at(i);
		test.process(context);
	}
}

ButterworthBandpass::ButterworthSixthOrder::~ButterworthSixthOrder() {};

void ButterworthBandpass::ButterworthSixthOrder::prepare(float lowFreq, float highFreq,const juce::dsp::ProcessSpec& spec, int order) {
	auto lowpassCoeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(highFreq, spec.sampleRate, order);

	lowpass.setBypassed<0>(true);
	lowpass.setBypassed<1>(true);
	lowpass.setBypassed<2>(true);

	*lowpass.get<0>().state = *lowpassCoeffs[0];
	*lowpass.get<1>().state = *lowpassCoeffs[1];
	*lowpass.get<2>().state = *lowpassCoeffs[2];

	lowpass.setBypassed<0>(false);
	lowpass.setBypassed<1>(false);
	lowpass.setBypassed<2>(false);

	lowpass.prepare(spec);

	auto highpassCoeffs = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(lowFreq, spec.sampleRate, order);

	highpass.setBypassed<0>(true);
	highpass.setBypassed<1>(true);
	highpass.setBypassed<2>(true);

	*highpass.get<0>().state = *highpassCoeffs[0];
	*highpass.get<1>().state = *highpassCoeffs[1];
	*highpass.get<2>().state = *highpassCoeffs[2];

	highpass.setBypassed<0>(false);
	highpass.setBypassed<1>(false);
	highpass.setBypassed<2>(false);

	highpass.prepare(spec);
}

void ButterworthBandpass::ButterworthSixthOrder::process(juce::dsp::ProcessContextReplacing<float> context) {
	highpass.process(context);
	lowpass.process(context);
};
void ButterworthBandpass::ButterworthSixthOrder::reset() {
	lowpass.reset();
	highpass.reset();
};

void ButterworthBandpass::Cascading::prepare(const juce::dsp::ProcessSpec& spec) {
	Duplicator::prepare(spec);
}
void ButterworthBandpass::Cascading::process(juce::dsp::ProcessContextReplacing<float> context) {
	Duplicator::process(context);
}
void ButterworthBandpass::Cascading::reset() {
	Duplicator::reset();
}

