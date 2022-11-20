
#include "ChannelFilterVisualizer.h"

//juce::Colour State::colours[State::maxNumChannels];


ChannelFilterVisualizer::ChannelFilterVisualizer() { };
ChannelFilterVisualizer::~ChannelFilterVisualizer() { };

void ChannelFilterVisualizer::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).darker(0.95F));

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);

    g.drawSingleLineText("Filters", 10, 20);

    if (frequencies.size() == 0 || magnitudes.size() == 0) return;

    int currN = State::GetDenormalizedValue("channelN");

    for (int channel = 0; channel < currN; channel++)
    {
        g.setColour(State::colours[channel]);
        g.drawSingleLineText("C" + std::to_string(channel + 1), 10 + 20 * (channel), 40);

        juce::Path path;
        
        double* freq = frequencies.at(channel);
        double* magn = magnitudes.at(channel);
        for (int i = 1; i < size; ++i)
        {
            auto width = getLocalBounds().getWidth();
            auto height = getLocalBounds().getHeight();

            path.addLineSegment(juce::Line<float>( 
                (float)juce::jmap((int)  freq[i - 1], 0,    size - 1, 0, width),
                       juce::jmap((float)magn[i - 1], 0.0f, 1.0f, (float)height - 10, 0.0f),
                (float)juce::jmap((int)  freq[i],     0,    size - 1, 0, width),
                       juce::jmap((float)magn[i],     0.0f, 1.0f, (float)height - 10, 0.0f)),1.0f);

        }

        g.fillPath(path.createPathWithRoundedCorners(5.f));
    }
}

void ChannelFilterVisualizer::addChannelFilter(int channel, double* _frequencies, double* _magnitudes) {

    if (channel == 0) {
        frequencies.clear();
        magnitudes.clear();
    }

    frequencies.push_back(_frequencies);
    magnitudes.push_back(_magnitudes);

    int currN = State::GetDenormalizedValue("channelN");

    if (channel >= currN) {
        repaint();
    }
}

void ChannelFilterVisualizer::resized() { }
