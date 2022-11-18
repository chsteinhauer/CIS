

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MediaPlayerComponent  : 
    public juce::Component, 
    public juce::Timer, 
    public juce::ChangeListener
{
public:
    MediaPlayerComponent();
    ~MediaPlayerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void updateLoopState(bool shouldLoop);

    void prepareMediaPlayer(int samplesPerBlockExpected, double sampleRate);
    void getMediaAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void stopMediaPlayer();

private:

    enum TransportState {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonChanged();
    void changeState(TransportState newState);
    
    TransportState state;
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::ToggleButton loopingToggle;
    juce::Label currentPositionLabel;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::String mediaLength;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MediaPlayerComponent)
};
