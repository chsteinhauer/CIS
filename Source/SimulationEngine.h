#pragma once

#include <JuceHeader.h>

template<typename ModuleA, typename ModuleB, typename ModuleC>
class SimulationEngine : public juce::AudioProcessor,
                         public juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>,
                         private juce::ChangeListener
{
public:
    SimulationEngine() : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo())
                                                         .withOutput("Output", AudioChannelSet::stereo()))
    {
        addParameter(gain = new AudioParameterFloat({ "gain", 1 }, "Gain", 0.0f, 1.0f, 0.5f));

    } {

    };

    void prepareToPlay(int blockSize, double sampleRate) override {
        this->prepare({ sampleRate, (juce::uint32)blockSize, 2 });
    };

    void releaseResources() override {
        this->reset();
    };

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        if (bufferToFill.buffer == nullptr)
        {
            jassertfalse;
            return;
        }

        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer,
            (size_t)bufferToFill.startSample);

        juce::ScopedLock audioLock(audioCallbackLock);
        this->process(juce::dsp::ProcessContextReplacing<float>(block));
    };

    void changeListenerCallback(juce::ChangeBroadcaster*) {
        juce::ScopedLock audioLock(audioCallbackLock);
    };

    //void getStateInformation(MemoryBlock& destData) override
    //{
    //    juce::MemoryOutputStream(destData, true).writeFloat(*gain);
    //}

    //void setStateInformation(const void* data, int sizeInBytes) override
    //{
    //    gain -> setValueNotifyingHost(juce::MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat());
    //}

    //==============================================================================
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override
    {
        const auto& mainInLayout = layouts.getChannelSet(true, 0);
        const auto& mainOutLayout = layouts.getChannelSet(false, 0);

        return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
    }

    juce::CriticalSection audioCallbackLock;
};