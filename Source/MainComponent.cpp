#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent(): juce::AudioAppComponent(otherDeviceManager)
{
    setLookAndFeel(&otherLookAndFeel);

    engine.reset();
    
    if (engine.get() == nullptr) {
        engine.reset(new SimulationEngine<Preprocess, Analyse, Reconstruction>());
    }

    //Reset settings pointer
    audioSettings.reset(new juce::AudioDeviceSelectorComponent(otherDeviceManager, 0, 2, 0, 2, false, false, true, false));
    otherDeviceManager.initialise(2, 2, nullptr, true);
    
    GUISetup();

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    setLookAndFeel(nullptr);
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    engine -> prepareToPlay(sampleRate, samplesPerBlockExpected);
    editor -> playerPanel.mediaPlayer.prepareMediaPlayer(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (bufferToFill.buffer == nullptr)
    {
        jassertfalse;
        return;
    }

    auto* device = deviceManager.getCurrentAudioDevice();
    auto inputChannels = device->getActiveInputChannels();
    auto outputChannels = device->getActiveOutputChannels();
    auto maxInputChannels = inputChannels.getHighestBit() + 1;
    auto maxOutputChannels = outputChannels.getHighestBit() + 1;

    if (editor->playerPanel.mediaToggle.getToggleState())
    {
        
        editor->playerPanel.mediaPlayer.getMediaAudioBlock(bufferToFill);
    }

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        // If there is no input or input and ouput channel do not match and not using media
        if ((!outputChannels[channel] || maxInputChannels == 0 || !inputChannels[channel])
            && editor->playerPanel.mediaToggle.getToggleState() == false)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }

        auto* buffer = bufferToFill.buffer->getReadPointer(channel, bufferToFill.startSample);

        visualizeInput(buffer, bufferToFill.numSamples);
    }

    // Audio processing goes here...
    if(State::GetDenormalizedValue("channelN") > 0)
        engine->processBlockSimulation((juce::dsp::AudioBlock<float>(*bufferToFill.buffer)));

    // Show the default output state if toggle state is false
    if (!editor->playerPanel.outToggle.getToggleState()) {
        for (auto channel = 0; channel < maxOutputChannels; ++channel)
        {
            auto* buffer = bufferToFill.buffer->getReadPointer(channel, bufferToFill.startSample);

            visualizeOutput(buffer, bufferToFill.numSamples);
        }
    }
}

void MainComponent::releaseResources()
{
    engine -> releaseResources();
    editor -> playerPanel.mediaPlayer.releaseResources();
}

void MainComponent::reset()
{
    engine->reset();
}

//==============================================================================
void MainComponent::GUISetup() {
    editor = engine->getEditor();
    addAndMakeVisible(editor);

    // Add audio settings
    audioSettings->setSize(600,400);
    editor->setupSettingsModal(audioSettings.get());

    // Initial size of application
    setSize(1400, 600);
}

void MainComponent::visualizeInput(const float* buffer, int size) {
    for (auto i = 0; i < size; ++i)
    {
        editor->playerPanel.IN.pushNextSampleIntoFifo(buffer[i]);
    }
}

void MainComponent::visualizeOutput(const float* buffer, int size) {
    for (auto i = 0; i < size; ++i)
    {
        editor->playerPanel.OUT.pushNextSampleIntoFifo(buffer[i]);
    }
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    editor->setBounds(getLocalBounds());
}

