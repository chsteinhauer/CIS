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
    setLookAndFeel(nullptr);
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

            auto* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
            

            for (auto i = 0; i < bufferToFill.numSamples; ++i)
            {
                auto volume = State::GetInstance()->getParameter("volume")->getValue();
                outBuffer[i] = buffer[i] * volume;
                OUT.pushNextSampleIntoFifo(outBuffer[i]);
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

    // Initial size of application
    setSize(1000, 600);
}


void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // Graph/Visualizer panel
    juce::FlexBox visualizePanel;
    visualizePanel.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    visualizePanel.alignContent = juce::FlexBox::AlignContent::center;
    visualizePanel.flexDirection = juce::FlexBox::Direction::column;

    visualizePanel.items.add(juce::FlexItem(IN).withMinWidth(300.0f).withMinHeight(200.0f));
    visualizePanel.items.add(juce::FlexItem(OUT).withMinWidth(300.0f).withMinHeight(200.0f));

    visualizePanel.performLayout(getLocalBounds().toFloat());

    juce::FlexBox content;
    content.alignContent = juce::FlexBox::AlignContent::center;
    content.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    content.items.add(juce::FlexItem(*editor).withMinWidth(200.0f).withMinHeight(400.0f).withFlex(1));
    content.items.add(juce::FlexItem(visualizePanel).withMinWidth(400.0f).withMinHeight(400.0f));

    content.performLayout(getLocalBounds().toFloat());

    // Main application panel
    juce::FlexBox main;
    main.alignContent = juce::FlexBox::AlignContent::center;
    main.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    main.flexDirection = juce::FlexBox::Direction::column;

    main.items.add(juce::FlexItem(content).withFlex(1));

    main.performLayout(getLocalBounds().toFloat());

}