

#pragma once

#include <JuceHeader.h>
#include "SimulationEditor.h"
#include "SimulationState.h"

template<typename ModuleA, typename ModuleB, typename ModuleC>
class SimulationEngine : public juce::AudioProcessor,
                         public juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>
{
public:
    SimulationEngine() : AudioProcessor(getBusesProperties()) {
        State::Initialize(*this);
    };
    ~SimulationEngine() {}
    

    void prepareToPlay(double sampleRate, int blockSize) override
    {
        this->prepare({ sampleRate, (juce::uint32)blockSize, 2 });
    }

    void releaseResources() override
    {
        this->juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>::reset();
    }

    void beginSimulationProcess(const juce::AudioSourceChannelInfo& bufferToFill)
    {
        jassert(!isUsingDoublePrecision());

        if (bufferToFill.buffer == nullptr)
        {
            jassertfalse;
            return;
        }

        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer,
            (size_t)bufferToFill.startSample);

        juce::ScopedLock audioLock(audioCallbackLock);
        this->process(juce::dsp::ProcessContextReplacing<float>(block));
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {};
    void processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override {};

    bool hasEditor() const override { return false; }

    SimulationEditor* getEditor()
    {
        return (SimulationEditor*)createEditor();
    }

    juce::AudioProcessorEditor* createEditor() override
    {
        return new SimulationEditor(*this);
    }

    //==============================================================================
    const juce::String getName() const override { return "CochlearImplantSimulation"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override { return 0; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "None"; }
    void changeProgramName(int, const juce::String&) override {}

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override { }

    void setStateInformation(const void* data, int sizeInBytes) override { }

    static BusesProperties getBusesProperties()
    {
        return BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true);
    }

    //==============================================================================
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override
    {
        const auto& mainInLayout = layouts.getChannelSet(true, 0);
        const auto& mainOutLayout = layouts.getChannelSet(false, 0);

        return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
    }
    

    juce::CriticalSection audioCallbackLock;
};