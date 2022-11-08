#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent(): juce::AudioAppComponent(otherDeviceManager)
{

    //Reset settings pointer
    audioSettings.reset(new juce::AudioDeviceSelectorComponent(otherDeviceManager, 0, 2, 0, 2, false, false, true, false));

    otherDeviceManager.initialise(2, 2, nullptr, true);
    
    // Add audio settings
    addAndMakeVisible(audioSettings.get());

    engine.reset();
    
    if (engine.get() == nullptr) {
        engine.reset(new SimulationEngine<PreprocessExample, AnalyseExample, ReconstructionExample>());
    }

    // Add input and output spectrum visualizer 
    addAndMakeVisible(IN);
    addAndMakeVisible(OUT);
    addAndMakeVisible(mediaPlayer);

    addAndMakeVisible(mediaToggle);
    mediaToggle.setButtonText("Use media as input");
    mediaToggle.onClick = [this] { mediaToggleButtonChanged(); };
    mediaPlayer.setEnabled(mediaToggle.getToggleState());

    IN.setTitle("Input");
    OUT.setTitle("Output");

    setSize (1000, 600);

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

////==============================================================================
//void MainComponent::initEngine()
//{
//    MainComponent::engine.reset();
//
//    if (MainComponent::engine.get() == nullptr) {
//        MainComponent::engine.reset(new SimulationEngine<PreprocessExample, AnalyseExample, ReconstructionExample>());
//    }
//}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    engine -> prepareToPlay(samplesPerBlockExpected, sampleRate);
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    mediaPlayer.prepareMediaPlayer(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
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

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        auto* buffer = bufferToFill.buffer->getReadPointer(channel, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            OUT.pushNextSampleIntoFifo(buffer[i]);
        }
    }
}

void MainComponent::releaseResources()
{
    engine -> releaseResources();
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    mediaPlayer.releaseResources();
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    

    // Spectrum visualizer
    int w = 400; int h = 200;
    IN.setBounds(getWidth() - w, getHeight() - h * 2, w, h);
    OUT.setBounds(getWidth() - w, getHeight() - h, w, h);

    // Settings
    audioSettings->setBounds(0, 0, getWidth() - w - 20, 300);

    // Media Player
    mediaPlayer.setBounds(getWidth() - w, 40, w, 160);
    mediaToggle.setBounds(getWidth() - w, 10, 150, 20);
    
}

void MainComponent::mediaToggleButtonChanged()
{
    mediaPlayer.setEnabled(mediaToggle.getToggleState());
    if (mediaToggle.getToggleState() == false)
    {
        mediaPlayer.stopMediaPlayer();
    }
}