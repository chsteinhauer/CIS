//
//
//#include "SimulationEngine.h"
//
//
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//SimulationEngine<ModuleA,ModuleB, ModuleC>::SimulationEngine() : AudioProcessor(getBusesProperties()) {
//    State::Initialize(*this);
//};
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//SimulationEngine<ModuleA, ModuleB, ModuleC>::~SimulationEngine() {}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::prepareToPlay(double sampleRate, int blockSize)
//{
//    State::WhenStateIsReady((std::function<void()>)[this, sampleRate, blockSize] {
//        auto N = State::GetInstance()->getParameter("channelN")->getValue();
//
//        tempBlock.reset(new juce::dsp::AudioBlock<float>(tempBlockMemory, N, blockSize));
//
//        this->prepare({ sampleRate, (juce::uint32)blockSize, 2 });
//    });
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::releaseResources()
//{
//    this->juce::dsp::ProcessorWrapper<juce::dsp::ProcessorChain<ModuleA, ModuleB, ModuleC>>::reset();
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::beginSimulationProcess(const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    jassert(!isUsingDoublePrecision());
//
//    if (bufferToFill.buffer == nullptr)
//    {
//        jassertfalse;
//        return;
//    }
//    if (!State::isActive()) return;
//
//    juce::dsp::AudioBlock<float> mainBlock(*bufferToFill.buffer, bufferToFill.startSample);
//
//    auto n = tempBlock->getNumChannels();
//    for (int i = 0; i < n; i++) {
//        auto&& buffer = bufferToFill.buffer;
//        juce::dsp::AudioBlock<float> block(*buffer, bufferToFill.startSample);
//
//        auto channel = tempBlock->getSingleChannelBlock(i);
//        channel.copyFrom(block);
//    }
//
//    juce::ScopedLock audioLock(audioCallbackLock);
//    this->process(juce::dsp::ProcessContextReplacing<float>(*tempBlock.get()));
//
//    mainBlock.fill(0);
//    juce::dsp::AudioBlock<float>& test(tempBlock->getSingleChannelBlock(0));
//    for (int i = 1; i < n; i++) {
//        auto tmp = tempBlock->getSingleChannelBlock(i);
//
//        tmp.multiplyBy((1.0f / test.getNumChannels()));
//        test.add(tmp);
//    }
//
//    mainBlock.copyFrom(test);
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {};
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) {};
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//bool SimulationEngine<ModuleA, ModuleB, ModuleC>::hasEditor() const { return false; }
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//SimulationEditor* SimulationEngine<ModuleA, ModuleB, ModuleC>::getEditor()
//{
//    return (SimulationEditor*)createEditor();
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//juce::AudioProcessorEditor* SimulationEngine<ModuleA, ModuleB, ModuleC>:: createEditor()
//{
//    return new SimulationEditor(*this);
//}
//
////==============================================================================
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//const juce::String SimulationEngine<ModuleA, ModuleB, ModuleC>::getName() const { return "CochlearImplantSimulation"; }
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//bool SimulationEngine<ModuleA, ModuleB, ModuleC>::acceptsMidi() const { return false; }
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//bool SimulationEngine<ModuleA, ModuleB, ModuleC>::producesMidi() const { return false; }
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//double SimulationEngine<ModuleA, ModuleB, ModuleC>::getTailLengthSeconds() const { return 0.0; }
//
////==============================================================================
//int getNumPrograms() { return 0; }
//int getCurrentProgram() { return 0; }
//void setCurrentProgram(int) {}
//const juce::String getProgramName(int) { return "None"; }
//void changeProgramName(int, const juce::String&) {}
//
////==============================================================================
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::getStateInformation(juce::MemoryBlock& destData) { }
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::setStateInformation(const void* data, int sizeInBytes) { }
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//juce::AudioProcessor::BusesProperties SimulationEngine<ModuleA, ModuleB, ModuleC>::getBusesProperties()
//{
//    return BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
//        .withOutput("Output", juce::AudioChannelSet::stereo(), true);
//}
//
////==============================================================================
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//bool SimulationEngine<ModuleA, ModuleB, ModuleC>::isBusesLayoutSupported(const BusesLayout& layouts) const
//{
//    const auto& mainInLayout = layouts.getChannelSet(true, 0);
//    const auto& mainOutLayout = layouts.getChannelSet(false, 0);
//
//    return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
//}
//
//
////template class SimulationEngine<PreprocessExample,AnalyseExample,ReconstructionExample>;
