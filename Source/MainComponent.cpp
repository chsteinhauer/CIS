#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent(): juce::AudioAppComponent(otherDeviceManager)
{
    engine.reset();
    
    if (engine.get() == nullptr) {
        engine.reset(new SimulationEngine<PreprocessExample, AnalyseExample, ReconstructionExample>());
    }

    //Reset settings pointer
    audioSettings.reset(new juce::AudioDeviceSelectorComponent(otherDeviceManager, 0, 2, 0, 2, false, false, true, true));
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
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    engine -> prepareToPlay(sampleRate, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* device = deviceManager.getCurrentAudioDevice();
    auto inputChannels = device->getActiveInputChannels();
    auto outputChannels = device->getActiveOutputChannels();
    auto maxInputChannels = inputChannels.getHighestBit() + 1;
    auto maxOutputChannels = outputChannels.getHighestBit() + 1;

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        if ((!outputChannels[channel]) || maxInputChannels == 0)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            auto actualInputChannel = channel % maxInputChannels;
        }

        if (!inputChannels[channel])
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            auto* buffer = bufferToFill.buffer -> getReadPointer(channel, bufferToFill.startSample);

            for (auto i = 0; i < bufferToFill.numSamples; ++i)
            {
                IN.pushNextSampleIntoFifo(buffer[i]);
            }

            engine -> beginSimulationProcess(bufferToFill);

            for (auto i = 0; i < bufferToFill.numSamples; ++i)
            {
                OUT.pushNextSampleIntoFifo(buffer[i]);
            }
        }
    }
}

void MainComponent::releaseResources()
{
    engine -> releaseResources();
}

//==============================================================================
void MainComponent::GUISetup() {
    // Add audio settings
    addAndMakeVisible(audioSettings.get());

    editor = engine->getEditor();
    addAndMakeVisible(editor);

    // Add input and output spectrum visualizer 
    addAndMakeVisible(IN);
    IN.setTitle("Input");
    addAndMakeVisible(OUT);
    OUT.setTitle("Output");

    // Initial size of application
    setSize(1000, 400);
}
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    //// Spectrum visualizer
    //int w = 400; int h = 200;
    //IN.setBounds(getWidth() - w, getHeight() - h * 2, w, h);
    //OUT.setBounds(getWidth() - w, getHeight() - h, w, h);

    //// Settings
    //audioSettings->setBounds(0, 0, getWidth() - w - 20, 30);

    juce::FlexBox freqVisualizers;
    freqVisualizers.alignContent = juce::FlexBox::AlignContent::center;
    freqVisualizers.flexDirection = juce::FlexBox::Direction::column;

    freqVisualizers.items.add(juce::FlexItem(IN).withMinWidth(400.0f).withMinHeight(200.0f));
    freqVisualizers.items.add(juce::FlexItem(OUT).withMinWidth(400.0f).withMinHeight(200.0f));

    freqVisualizers.performLayout(getLocalBounds().toFloat());


    juce::FlexBox main;
    //main.flexWrap = juce::FlexBox::Wrap::wrap;
    main.alignContent = juce::FlexBox::AlignContent::center;
    main.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    main.items.add(juce::FlexItem(*editor).withMinWidth(400.0f).withMinHeight(400.0f));

    if (State::GetInstance() -> getParameter("settings") -> getValue()) {
        main.items.add(juce::FlexItem(*audioSettings).withMinWidth(400.0f).withMinHeight(400.0f));
    }

    main.items.add(juce::FlexItem(freqVisualizers).withMinWidth(600.0f).withMinHeight(400.0f));

    main.performLayout(getLocalBounds().toFloat());
    
}