
#include "ButterworthBandpass.h"
#include "SimulationState.h"
#include "Biquad.h"
#include "Butterworth.h"

ButterworthBandpass::ButterworthBandpass() {
	//lowPassArray.reset(new FilterVector());
	//highPassArray.reset(new FilterVector());
	//bandpasses.reset(new FilterVector());
}
ButterworthBandpass::~ButterworthBandpass() {}

void ButterworthBandpass::remakeFilters(const juce::dsp::ProcessSpec& spec)
{

	//std::vector<Biquad2> coeffs;
	//Butterworth butterworth;
	//clearFilters();


	int N = spec.numChannels;
	std::vector<ButterworthSixthOrder> tmp(N);

	auto gw = State::GetInstance()->getParameter("Greenwood");

	float lowFreq = gw->convertFrom0to1(0); 
	float highFreq;
	for (int i = 1; i <= N; i++)
	{
		highFreq = gw->convertFrom0to1(((float)i / N));
		tmp.at((size_t)i-1).prepare(lowFreq, highFreq, { spec.sampleRate, (juce::uint32)spec.maximumBlockSize, (juce::uint32)1 });

		lowFreq = highFreq;

		//double gain = 1.0;
		//coeffs.clear();

		//// array of biquad filters (for this case, array size = 4 )
		//butterworth.bandPass(samplingFrequency,  // fs
		//					lowFreq,    // freq1
		//					highFreq,   // freq2. N/A for lowpass
		//					6, 			// filter order,
		//					coeffs,		// coefficient array being filled
		//					gain);		// overall gain

		

		/*std::vector<juce::dsp::IIR::Filter<float>*> bandpass;
		for (int i = 0; i < coeffs.size(); i++) {
			float b0 = coeffs[i].b0;
			float b1 = coeffs[i].b1;
			float b2 = coeffs[i].b2;
			float b3 = coeffs[i].b3;

			float a0 = 1.0f;
			float a1 = coeffs[i].a1;
			float a2 = coeffs[i].a2;
			float a3 = coeffs[i].a3;
			auto coeff = juce::dsp::IIR::Coefficients<float>(b0, b1, b2, b3, a0, a1, a2, a3);
			bandpass.push_back(new juce::dsp::IIR::Filter<float>());
		}

		bandpasses->push_back(bandpass);*/
		

		//auto lowpassCoeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(lowFreq, samplingFrequency, 6);
		//
		//for (int i = 0; i < 3; i++) {
		//	lowpass[i] = new juce::dsp::IIR::Filter<float>(lowpassCoeffs[i]);
		//}
		//lowPassArray.push_back(lowpass);

		//auto highpassCoeffs = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(highFreq, samplingFrequency, 6);
		//for (int i = 0; i < 3; i++) {
		//	highpass[i] = juce::dsp::IIR::Filter<float>(highpassCoeffs[i]);
		//}
		//highPassArray.push_back(highpass);

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
	//lowPassArray.clear();
	//highPassArray.clear();

	//lowPassArray.reset(new FilterVector());
	//highPassArray.reset(new FilterVector());

	/*for (int i = 0; i < bandpasses->size(); i++)
	{
		bandpasses->at(i).clear();
	}
	bandpasses.reset(new FilterVector());*/
}

void ButterworthBandpass::process(juce::dsp::AudioBlock<float> block)
{
	int N = block.getNumChannels();

	for (int i = 0; i < N; i++) {
		juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));

		auto& test = bandpasses.at(i);
		test.process(context);

		//context.getOutputBlock().multiplyBy(1 + 50 * (1/State::GetDenormalizedValue("channelN")));


		//auto lowpass = lowPassArray.at(i);
		//auto highpass = highPassArray.at(i);
		//const auto bandpass = bandpasses->at(i);

		//juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));
		//auto lp = lowpass->data();
		
	/*	for (juce::OwnedArray<juce::dsp::IIR::Filter<float>>::iterator ptr = lowpass->begin(); ptr < lowpass->end(); ptr++) {

		}*/


		//auto hp = highpass.data();
		//for (int j = 0; j < block.getNumSamples(); j++)
		//{
		//	/*if(hp != nullptr)
		//		hp[k]->process(context);*/
		//	//block.getSingleChannelBlock(i).multiplyBy(3);

		//	int test = lowpass.size();
		//	for (int k = 0; k < lowpass.size(); k++)
		//	{
		//		//if (lp != nullptr)
		//		//lowpass.at(k).processSample(block.getSample(i,j));
		//			//block.getSingleChannelBlock(i).multiplyBy(3);
		//	}

		//	int test2 = highpass.size();
		//	for (int k = 0; k < highpass.size(); k++)
		//	{
		//		//if(hp != nullptr)
		//		//highpass.at(k).processSample(block.getSample(i, j));
		//			//block.getSingleChannelBlock(i).multiplyBy(3);
		//	}
		//}
	}
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

//ButterworthBandpass::ButterworthSixthOrder::ButterworthSixthOrder() {};
ButterworthBandpass::ButterworthSixthOrder::~ButterworthSixthOrder() {};

//ButterworthBandpass::ButterworthSixthOrder::ButterworthSixthOrder(ButterworthSixthOrder&) {
//}
//ButterworthBandpass::ButterworthSixthOrder::ButterworthSixthOrder(const ButterworthSixthOrder&) {
//}

void ButterworthBandpass::ButterworthSixthOrder::prepare(float lowFreq, float highFreq,const juce::dsp::ProcessSpec& spec) {
	auto lowpassCoeffs = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(lowFreq, spec.sampleRate, 6);

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

	auto highpassCoeffs = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(highFreq, spec.sampleRate, 6);

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

	//context.getOutputBlock().multiplyBy(1.5 + 2 * (1/State::GetDenormalizedValue("channelN")));
}
void ButterworthBandpass::Cascading::reset() {
	Duplicator::reset();
}

