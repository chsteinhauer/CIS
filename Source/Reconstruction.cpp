

#include "Reconstruction.h"

Reconstruction::Reconstruction() { }
Reconstruction::~Reconstruction() { }

void Reconstruction::parameterChanged(const juce::String& parameterID, float newValue) {
	/*if (parameterID == "threshold") {
		compressor.setThreshold(newValue);
	}

	if (parameterID == "makeupgain") {
		makeupGain.setGainDecibels(newValue);

		DBG("gain: " << makeupGain.getGainLinear());
	}*/
}

void Reconstruction::prepare(const juce::dsp::ProcessSpec& spec) {
	if (spec.numChannels <= 0) {
		return;
	}

	if (!initialized) {
		/*State::GetInstance()->addParameterListener("threshold", this);
		State::GetInstance()->addParameterListener("makeupgain", this);*/

		initialized = true;
	}

	synth.prepare(spec);
	butterworth.remakeFilters(spec);
	/*compressor.reset();
	expander.reset();
	makeupGain.reset();
	
	auto th = State::GetDenormalizedValue("threshold");
	compressor.setThreshold(th);
	compressor.setRatio(12);
	compressor.setAttack(3);
	compressor.setRelease(100);
	compressor.prepare(spec);

	expander.setThreshold(th - 30);
	expander.setRatio(1.9);
	expander.setAttack(3);
	expander.setRelease(100);
	expander.prepare(spec);

	auto mg = State::GetDenormalizedValue("makeupgain");
	makeupGain.setGainDecibels(mg);
	makeupGain.prepare(spec);*/
}

void Reconstruction::process(const juce::dsp::ProcessContextReplacing<float>& context) {
	if (context.isBypassed) {
		return;
	}

	synth.process(context);
	butterworth.process(context.getOutputBlock());
	/*compressor.process(context);
	expander.process(context);
	makeupGain.process(context);*/
}

void Reconstruction::reset() {
	synth.reset();
	butterworth.clearFilters();
	/*compressor.reset();
	makeupGain.reset();
	expander.reset();*/
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

	float sineGain = State::GetDenormalizedValue("sinegain");
	float noiseGain = State::GetDenormalizedValue("noisegain");
	float pshcGain = State::GetDenormalizedValue("pshcgain");

	for (int channel = 0; channel < block.getNumChannels(); channel++)
	{
		float* data = block.getChannelPointer(channel);

		for (int i = 0; i < block.getNumSamples(); i++)
		{
			// carriers
			float csine = 0, cnoise = 0, cpshc = 0;

			if (sineEnabled) 
				csine = sine.next(channel) * sineGain;

			if (noiseEnabled)
				cnoise = noise.next() * noiseGain;

			if (pshcEnabled)
				cpshc = pshc.next(channel) * pshcGain;

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
