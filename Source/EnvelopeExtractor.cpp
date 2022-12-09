

#include "EnvelopeExtractor.h"
#include "SimulationState.h"

EnvelopeExtractor::EnvelopeExtractor() 
{
    
}

void EnvelopeExtractor::prepareHalfwaveRectification(const juce::dsp::ProcessSpec& spec) {
    filters.clear();
    auto gw = State::GetInstance()->getParameter("Greenwood");

    float f = gw->convertFrom0to1(0);

    int N = spec.numChannels;

    // Calculate lowpass cutoff based on Mesnildrey et al. (2016)
    for (int i = 0; i < N; i++) {
        const float fkHz = f * 0.001;
        float pps = 37 + 151 * fkHz + 0.17 * pow(fkHz, 2);
        float fcutoff = std::fmin(pps*0.5, 200);

        auto coeffs =  juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(fcutoff, spec.sampleRate, 2);
        filters.push_back(std::make_unique<juce::dsp::IIR::Filter<float>>(coeffs[0]));

        f = gw->convertFrom0to1(static_cast<float>(i + 1) / N);
    }

    // Extra additional lowpass filter bc the envelope seems to be too detailed
    iir.reset();
    iir.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, 50, 2);
    iir.prepare(spec);
}

void EnvelopeExtractor::halfwaveRectification(const juce::dsp::ProcessContextReplacing<float>& context)
{
    juce::dsp::AudioBlock<float> block(context.getOutputBlock());

    for (int i = 0; i < block.getNumChannels(); i++)
    {
        float* data = block.getChannelPointer(i);
        for (int j = 0; j < block.getNumSamples(); j++)
        {

            auto sample = data[j];
            auto half_rec = std::fmaxf(sample,0);
            auto env = filters.at(i)->processSample(half_rec);
            filters.at(i)->snapToZero();

            // make envelope amplitude the same as input signal amplitude
            //data[j] = sqrtf(static_cast<float>(2.f) * env);

            data[j] = env;
        }
    }

    iir.process(context);
}

void EnvelopeExtractor::hilbertTransform(juce::dsp::AudioBlock<float> block)
{
    
}

void EnvelopeExtractor::reset() {
    for (auto& filter : filters) {
        filter->reset();
    }

    iir.reset();
}