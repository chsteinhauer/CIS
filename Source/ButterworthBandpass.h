

#pragma once
#include <JuceHeader.h>

using FilterArray = juce::OwnedArray<juce::OwnedArray<juce::dsp::IIR::Filter<float>>>;
using FilterVector = std::vector<std::array<juce::dsp::IIR::Filter<float>,3>>;

#pragma once
class ButterworthBandpass
{
public:
    ButterworthBandpass();
    ~ButterworthBandpass();

    void remakeFilters(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float> block);
    void clearFilters();

private:
    using Duplicator = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

    struct Cascading : Duplicator {
        void prepare(const juce::dsp::ProcessSpec& spec);
        void process(juce::dsp::ProcessContextReplacing<float> context);
        void reset();
    };

    struct ButterworthSixthOrder {
        ButterworthSixthOrder() = default;
        ~ButterworthSixthOrder();
        
        void prepare(float lowFreq, float highFreq, const juce::dsp::ProcessSpec& spec, int order);
        void process(juce::dsp::ProcessContextReplacing<float> context);
        void reset();

        juce::dsp::ProcessorChain<Cascading, Cascading, Cascading> lowpass;
        juce::dsp::ProcessorChain<Cascading, Cascading, Cascading> highpass;
    };

    /*FilterVector lowPassArray;
    FilterVector highPassArray;*/

    std::vector<ButterworthSixthOrder> bandpasses;

    float greenwood(float x);

    bool randomize;
    juce::Random random;

    //std::unique_ptr<FilterVector> bandpasses;
};