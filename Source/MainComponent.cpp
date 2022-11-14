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
    mediaPlayer.prepareMediaPlayer(samplesPerBlockExpected, sampleRate);
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

    if (mediaToggle.getToggleState())
    {
        mediaPlayer.getMediaAudioBlock(bufferToFill);
    }

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {

        //If there is no input or input and ouput channel do not match and not using media
        if ((!outputChannels[channel] || maxInputChannels == 0 || !inputChannels[channel])
            && mediaToggle.getToggleState() == false)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }

        auto* buffer = bufferToFill.buffer->getReadPointer(channel, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            IN.pushNextSampleIntoFifo(buffer[i]);
        }
    }

    //Audio processing goes here...
    engine->beginSimulationProcess(bufferToFill);

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            auto volume = State::GetInstance()->getParameter("volume")->getValue();
            auto audio = State::GetInstance()->getParameter("audio")->getValue();

            buffer[i] = buffer[i] * volume * !audio;

            OUT.pushNextSampleIntoFifo(buffer[i]);
        }
    }
}

void MainComponent::releaseResources()
{
    engine -> releaseResources();
    mediaPlayer.releaseResources();
}

//==============================================================================
void MainComponent::GUISetup() {
    editor = engine->getEditor();
    addAndMakeVisible(editor);

    // Add audio settings
    audioSettings->setSize(600,400);
    editor->setupSettingsModal(audioSettings.get());

    // Add input and output spectrum visualizer 
    addAndMakeVisible(IN);
    IN.setTitle("Input");
    addAndMakeVisible(OUT);
    OUT.setTitle("Output");

    addAndMakeVisible(mediaPlayer);

    addAndMakeVisible(mediaToggle);
    mediaToggle.setButtonText("Use media as input");
    mediaToggle.onClick = [this] { mediaToggleButtonChanged(); };
    mediaPlayer.setEnabled(mediaToggle.getToggleState());

    // Initial size of application
    setSize(1000, 600);
}


void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // Player panel
    juce::FlexBox player;
    player.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    player.alignContent = juce::FlexBox::AlignContent::center;
    player.flexDirection = juce::FlexBox::Direction::column;

    // Media Player
    player.items.add(juce::FlexItem(mediaToggle).withMinWidth(150).withMinHeight(20));
    player.items.add(juce::FlexItem(mediaPlayer).withMinWidth(400).withMinHeight(160));
    player.items.add(juce::FlexItem(IN).withMinWidth(300.0f).withMinHeight(200.0f));
    player.items.add(juce::FlexItem(OUT).withMinWidth(300.0f).withMinHeight(200.0f));

    player.performLayout(getLocalBounds().toFloat());

    juce::FlexBox content;
    content.alignContent = juce::FlexBox::AlignContent::center;
    content.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    content.items.add(juce::FlexItem(*editor).withMinWidth(200.0f).withMinHeight(400.0f).withFlex(1));
    content.items.add(juce::FlexItem(player).withMinWidth(400.0f).withMinHeight(400.0f));

    content.performLayout(getLocalBounds().toFloat());

    // Main application panel
    juce::FlexBox main;
    main.alignContent = juce::FlexBox::AlignContent::center;
    main.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    main.flexDirection = juce::FlexBox::Direction::column;

    main.items.add(juce::FlexItem(content).withFlex(1));

    main.performLayout(getLocalBounds().toFloat());
}

void MainComponent::mediaToggleButtonChanged()
{
    mediaPlayer.setEnabled(mediaToggle.getToggleState());
    if (mediaToggle.getToggleState() == false)
    {
        mediaPlayer.stopMediaPlayer();
    }
}