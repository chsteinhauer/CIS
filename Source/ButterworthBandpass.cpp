
#include "ButterworthBandpass.h"
#include "SimulationState.h"

ButterworthBandpass::ButterworthBandpass() {
	lowPassArray.reset(new FilterVector());
	highPassArray.reset(new FilterVector());
}
ButterworthBandpass::~ButterworthBandpass() {}

void ButterworthBandpass::remakeFilters(int numChannels, int samplingFrequency)
{
	//clearFilters();

	std::vector<float> freqs;
	float lowFreq = 20.0;
	float highFreq = State::GetInstance()->getParameter("Greenwood")->convertFrom0to1(0);
	int i = 1;
	for (;;)
	{
		auto lowpassCoeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(lowFreq, samplingFrequency, 6);
		
		std::vector<juce::dsp::IIR::Filter<float>*> lowpass;
		std::vector<juce::dsp::IIR::Filter<float>*> highpass;
		for (auto coeff : lowpassCoeffs) {
			lowpass.push_back(new juce::dsp::IIR::Filter<float>(coeff));
		}
		lowPassArray->push_back(lowpass);

		auto highpassCoeffs = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(highFreq, samplingFrequency, 6);
		for (auto coeff : highpassCoeffs) {
			highpass.push_back(new juce::dsp::IIR::Filter<float>(coeff));
		}
		highPassArray->push_back(highpass);

		if (i > numChannels - 1)
		{
			break;
		}

		lowFreq = highFreq;
		highFreq = State::GetInstance()->getParameter("Greenwood")->convertFrom0to1(static_cast<float>(i) / (numChannels-1)); //greenwood((float)i / (numChannels - 1));
		i++;
	}
}

void ButterworthBandpass::clearFilters()
{
	for (int i = 0; i < lowPassArray->size(); i++)
	{
		lowPassArray->at(i).clear();
	}
	lowPassArray->clear();
	for (int i = 0; i < highPassArray->size(); i++)
	{
		highPassArray->at(i).clear();
	}
	highPassArray->clear();

	lowPassArray.reset(new FilterVector());
	highPassArray.reset(new FilterVector());
}

void ButterworthBandpass::process(juce::dsp::AudioBlock<float> block)
{
	int N = block.getNumChannels();

	for (int i = 0; i < N; i++) {
		const auto lowpass = lowPassArray->at(i);
		const auto highpass = highPassArray->at(i);

		if (lowpass.size() != highpass.size())
			jassertfalse;

		juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));
		for (int j = 0; j < lowpass.size(); j++)
		{
			lowpass.at(j)->process(context);
			highpass.at(j)->process(context);
		}
	}

	// Some gains to actually hear something, scaled by channels
	block.multiplyBy(150 + 10000 * exp(-N * 0.15));
}

float ButterworthBandpass::greenwood(float x)
{
	//Constants for greenwood function applied to the human cochlear
	const float A = 165.4f;
	const float a = 2.1f;
	const float K = 0.88f;

	if (x > 1 || x < 0)
	{
		return -1;
	}
	return A * (pow(10, (a * x)) - K);
}