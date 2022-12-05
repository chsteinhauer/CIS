#include <JuceHeader.h>
#include "MediaPlayerComponent.h"

//==============================================================================
MediaPlayerComponent::MediaPlayerComponent() : state(Stopped)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    //Media player buttons
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    addAndMakeVisible(&loopingToggle);
    loopingToggle.setButtonText("Loop");
    loopingToggle.onClick = [this] { loopButtonChanged(); };

    addAndMakeVisible(&currentPositionLabel);
    currentPositionLabel.setText("Stopped", juce::dontSendNotification);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    startTimer(20);
}

MediaPlayerComponent::~MediaPlayerComponent()
{
}

void MediaPlayerComponent::prepareMediaPlayer(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MediaPlayerComponent::getMediaAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
}

void MediaPlayerComponent::releaseResources()
{
    transportSource.releaseResources();
}

void MediaPlayerComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void MediaPlayerComponent::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    loopingToggle.setBounds(10, 100, getWidth() - 20, 20);
    currentPositionLabel.setBounds(10, 130, getWidth() - 20, 20);
}

void MediaPlayerComponent::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
        juce::File{},
        "*.wav, *.mp3");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                auto* reader = formatManager.createReaderFor(file);

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                    playButton.setEnabled(true);
                    readerSource.reset(newSource.release());
                }
            }
        });
}

void MediaPlayerComponent::playButtonClicked()
{
    updateLoopState(loopingToggle.getToggleState());
    changeState(Starting);
}

void MediaPlayerComponent::stopButtonClicked()
{
    stopMediaPlayer();
}

void MediaPlayerComponent::stopMediaPlayer()
{
    changeState(Stopping);
}

void MediaPlayerComponent::loopButtonChanged()
{
    updateLoopState(loopingToggle.getToggleState());
}

void MediaPlayerComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:
            stopButton.setEnabled(false);
            playButton.setEnabled(true);
            transportSource.setPosition(0.0);
            break;

        case Starting:
            playButton.setEnabled(false);
            transportSource.start();
            break;

        case Playing:
            stopButton.setEnabled(true);
            break;

        case Stopping:
            transportSource.stop();
            break;
        }
    }
}

void MediaPlayerComponent::updateLoopState(bool shouldLoop)
{
    if (readerSource.get() != nullptr)
        readerSource->setLooping(shouldLoop);
}

void MediaPlayerComponent::timerCallback()
{
    if (transportSource.isPlaying())
    {
        juce::RelativeTime position(transportSource.getCurrentPosition());

        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;
        auto millis = ((int)position.inMilliseconds()) % 1000;

        if (mediaLength.isEmpty())
        {
            juce::RelativeTime end(transportSource.getLengthInSeconds());

            auto endMinutes = ((int)end.inMinutes()) % 60;
            auto endSeconds = ((int)end.inSeconds()) % 60;
            auto endMillis = ((int)end.inMilliseconds()) % 1000;

            mediaLength = juce::String::formatted("%02d:%02d:%03d", endMinutes, endSeconds, endMillis);
        }

        auto positionString = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);

        currentPositionLabel.setText(positionString << "/" << mediaLength, juce::dontSendNotification);
    }
    else
    {
        currentPositionLabel.setText("Stopped", juce::dontSendNotification);
        mediaLength.clear();
    }
}

void MediaPlayerComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
    }
}