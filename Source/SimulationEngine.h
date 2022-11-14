

#pragma once

#include <JuceHeader.h>
#include "SimulationEditor.h"
#include "SimulationState.h"
#include "AnalyseExample.h"
#include "PreprocessExample.h"
#include "ReconstructionExample.h"

template<typename ModuleA, typename ModuleB, typename ModuleC>
class SimulationEngine : public juce::AudioProcessor,
                         public juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>
{
public:
    SimulationEngine() : AudioProcessor(getBusesProperties()) {
        State::Initialize(*this);
    }
    ~SimulationEngine() {}

    void prepareToPlay(double sampleRate, int blockSize) override
    {
        auto N = State::GetDenormalizedValue("channelN");
        this->prepare({ sampleRate, (juce::uint32)blockSize, (juce::uint32)N });

        tempBlock.reset(new juce::dsp::AudioBlock<float>(tempBlockMemory, N, blockSize));
    }

    void releaseResources() override
    {
        this->juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>::reset();
    }

    void beginSimulationProcess(const juce::AudioSourceChannelInfo& bufferToFill)
    {

        jassert(!isUsingDoublePrecision());

        // Store original buffer pointer in output block
        juce::dsp::AudioBlock<float> outputBlock(*bufferToFill.buffer, bufferToFill.startSample);

        // Copy content of main block to N amount of channels in tempBlock
        copyBlockToNChannels(bufferToFill);

        juce::ScopedLock audioLock(audioCallbackLock);
        // Run the simulation...!
        this->process(juce::dsp::ProcessContextReplacing<float>(*tempBlock.get()));

        // Pack processed data back to the original amount of channels
        auto simulatedBlock = packBlockToOrgChannels();

        // Now outputBlock may copy the results
        outputBlock.copyFrom(simulatedBlock);
    }

    bool hasEditor() const override { return true; }

    SimulationEditor* getEditor()
    {
        return (SimulationEditor*)createEditor();
    }


    juce::AudioProcessorEditor* createEditor() override
    {
        return new SimulationEditor(*this);
    }
    
    /*Ignore this section, is they are primarily there to allow SimulatorEngine to inherit AudioProcessor
    (however if we want to do something with MIDI, we can have a look at this)*/
    //==============================================================================
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {};
    void processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override {};
    const juce::String getName() const override { return "CochlearImplantSimulation"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 0; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "None"; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock& destData) override {}
    void setStateInformation(const void* data, int sizeInBytes) override {}
    //==============================================================================
    
    static BusesProperties getBusesProperties()
    {
        return BusesProperties().withInput("Input", juce::AudioChannelSet::mono(), true)
            .withOutput("Output", juce::AudioChannelSet::mono(), true);
    }

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override
    {
        const auto& mainInLayout = layouts.getChannelSet(true, 0);
        const auto& mainOutLayout = layouts.getChannelSet(false, 0);

        return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
    }

private:

    juce::dsp::AudioBlock<float> packBlockToOrgChannels() {
        // Add the single channels together in simulatedBlock
        juce::dsp::AudioBlock<float>& simulatedBlock(tempBlock->getSingleChannelBlock(0));
        for (int i = 1; i < tempBlock->getNumChannels(); i++) {
            auto tmp = tempBlock->getSingleChannelBlock(i);

            // multiply with the fraction of number of channels to not blow our ears off
            tmp.multiplyBy((1.0f / simulatedBlock.getNumChannels()));
            simulatedBlock.add(tmp);
        }

        return simulatedBlock;
    }

    void copyBlockToNChannels(juce::AudioSourceChannelInfo bufferToFill) {
        auto N = tempBlock->getNumChannels();
        for (int i = 0; i < N; i++) {
            auto&& buffer = bufferToFill.buffer;
            juce::dsp::AudioBlock<float> block(*buffer, bufferToFill.startSample);

            auto channel = tempBlock->getSingleChannelBlock(i);
            channel.copyFrom(block);
        }
    }

    juce::CriticalSection audioCallbackLock;
    juce::HeapBlock<char> tempBlockMemory;
    std::unique_ptr<juce::dsp::AudioBlock<float>> tempBlock;
};


// C++ is rude regarding templates and linking, but I'm leaving a commented out declare version here so I don't
// have to write it again if a solution is found

//SimulationEngine();
//~SimulationEngine();

//void prepareToPlay(double sampleRate, int blockSize) override;

//void releaseResources() override;

//void beginSimulationProcess(const juce::AudioSourceChannelInfo& bufferToFill);

//void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
//void processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override;

//bool hasEditor() const override;

//SimulationEditor* getEditor();

//juce::AudioProcessorEditor* createEditor() override;
//

////==============================================================================
//const juce::String getName() const override;
//bool acceptsMidi() const override;
//bool producesMidi() const override;
//double getTailLengthSeconds() const override;

////==============================================================================
//int getNumPrograms() override;
//int getCurrentProgram() override;
//void setCurrentProgram(int) override;
//const juce::String getProgramName(int) override;
//void changeProgramName(int, const juce::String&) override;

////==============================================================================
//void getStateInformation(juce::MemoryBlock& destData) override;

//void setStateInformation(const void* data, int sizeInBytes) override;

//static BusesProperties getBusesProperties();

////==============================================================================
//bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

