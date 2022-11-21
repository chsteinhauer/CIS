#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent(): juce::AudioAppComponent(otherDeviceManager)
{
    setLookAndFeel(&otherLookAndFeel);

    engine.reset();
    
    if (engine.get() == nullptr) {
        engine.reset(new SimulationEngine<PreprocessExample, AnalyseExample, ReconstructionExample>());
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
    editor ->playerPanel.mediaPlayer.prepareMediaPlayer(samplesPerBlockExpected, sampleRate);
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
        //If there is no input or input and ouput channel do not match and not using media
        if ((!outputChannels[channel] || maxInputChannels == 0 || !inputChannels[channel])
            && editor->playerPanel.mediaToggle.getToggleState() == false)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }

        auto* buffer = bufferToFill.buffer->getReadPointer(channel, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            editor->playerPanel.IN.pushNextSampleIntoFifo(buffer[i]);
        }
    }

    //Audio processing goes here...
    engine->beginSimulationProcess(bufferToFill);

    int N = State::GetDenormalizedValue("channelN");
    auto volume = State::GetInstance()->getParameter("volume")->getValue();
    auto audio = State::GetInstance()->getParameter("audio")->getValue();

    gain.setGainLinear(volume);
    if (!editor->playerPanel.outToggle.getToggleState()) {

        for (auto channel = 0; channel < maxOutputChannels; ++channel)
        {
            auto* buffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

            for (auto i = 0; i < bufferToFill.numSamples; ++i)
            {
                //buffer[i] = buffer[i] * volume * (N > 0 ? 1 : 0.3) * !audio;
                editor->playerPanel.OUT.pushNextSampleIntoFifo(buffer[i]);
            }
        }
    }
}

void MainComponent::releaseResources()
{
    engine -> releaseResources();
    editor -> playerPanel.mediaPlayer.releaseResources();
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

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    editor->setBounds(getLocalBounds());
}

