#pragma once

#include <JuceHeader.h>

template<typename ModuleA, typename ModuleB, typename ModuleC>
class SimulationEngine : public juce::AudioSource,
                         public juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>,
                         private juce::ChangeListener
{
public:
    SimulationEngine() {

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

    juce::CriticalSection audioCallbackLock;
};