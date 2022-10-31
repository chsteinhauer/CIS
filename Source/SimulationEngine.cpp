//
//
//#include "SimulationEngine.h"
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//SimulationEngine<ModuleA,ModuleB,ModuleC>::SimulationEngine() {
//
//}
//
////template<typename ModuleA, typename ModuleB, typename ModuleC>
////SimulationEngine<ModuleA,ModuleB,ModuleC>::~SimulationEngine() {
////
////}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::prepareToPlay(int blockSize, double sampleRate)
//{
//    this->prepare({ sampleRate, (juce::uint32)blockSize, 2 });
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::releaseResources()
//{
//
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    if (bufferToFill.buffer == nullptr)
//    {
//        jassertfalse;
//        return;
//    }
//
//    juce::dsp::AudioBlock<float> block(*bufferToFill.buffer,
//        (size_t)bufferToFill.startSample);
//
//    juce::ScopedLock audioLock(audioCallbackLock);
//    this->process(juce::dsp::ProcessContextReplacing<float>(block));
//}
//
//template<typename ModuleA, typename ModuleB, typename ModuleC>
//void SimulationEngine<ModuleA, ModuleB, ModuleC>::changeListenerCallback(juce::ChangeBroadcaster*)
//{
//    juce::ScopedLock audioLock(audioCallbackLock);
//}