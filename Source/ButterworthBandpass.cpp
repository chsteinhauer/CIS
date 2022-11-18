
#include "ButterworthBandpass.h"
#include "SimulationState.h"
#include "Biquad.h"
#include "Butterworth.h"

ButterworthBandpass::ButterworthBandpass() {
	lowPassArray.reset(new FilterVector());
	highPassArray.reset(new FilterVector());
	//bandpasses.reset(new FilterVector());
}
ButterworthBandpass::~ButterworthBandpass() {}

void ButterworthBandpass::remakeFilters(int numChannels, int samplingFrequency)
{

	//std::vector<Biquad2> coeffs;
	//Butterworth butterworth;

	auto gw = State::GetInstance()->getParameter("Greenwood");

	float lowFreq = gw->convertFrom0to1(0); 
	float highFreq;
	for (int i = 1; i <= numChannels; i++)
	{
		highFreq = gw->convertFrom0to1(((float)i / (numChannels)));

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

		lowFreq = highFreq;

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
		const auto lowpass = lowPassArray->at(i);
		const auto highpass = highPassArray->at(i);
		//const auto bandpass = bandpasses->at(i);

		/*auto channelData = block.getChannelPointer(i);
		for (int j = 0; j < block.getNumSamples(); j++) {*/

		if (lowpass.size() != highpass.size())
			jassertfalse;
		juce::dsp::ProcessContextReplacing<float> context(block.getSingleChannelBlock(i));
		for (int k = 0; k < lowpass.size(); k++)
		{
			//bandpass.at(k)->process(context);
			lowpass.at(k)->process(context);
			highpass.at(k)->process(context);
			//channelData[j] -= lowpass.at(k)->processSample(channelData[j]);
			//channelData[j] -= highpass.at(k)->processSample(channelData[j]);
			//channelData[j] = bandpass.at(k)->processSample(channelData[j]);

			block.getSingleChannelBlock(i).multiplyBy(3);
		}
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

