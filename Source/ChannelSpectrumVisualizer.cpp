

#include "ChannelSpectrumVisualizer.h"

juce::Colour State::colours[State::maxNumChannels];

ChannelSpectrumVisualizer::ChannelSpectrumVisualizer() : forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    setOpaque(true);
    startTimerHz(30);
    setSize(400, 200);
}

ChannelSpectrumVisualizer::~ChannelSpectrumVisualizer()
{

}

void ChannelSpectrumVisualizer::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).darker(0.95F));

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);
    drawFrame(g);
}

void ChannelSpectrumVisualizer::timerCallback()
{
    int currN = State::GetDenormalizedValue("channelN");

    bool shouldRepaint = false;


    for (int i = 0; i < std::min(currN,maxN); i++) {

        if (blockReady[i])
        {
            drawNextMultiFrameOfSpectrum(i);
            blockReady[i] = false;
            shouldRepaint = true;
        }
    }

    if (shouldRepaint)
        repaint();
}

void ChannelSpectrumVisualizer::drawNextMultiFrameOfSpectrum(int channel)
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable(fftTable[channel], fftSize);

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftTable[channel]);

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
        auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftTable[channel][fftDataIndex])
            - juce::Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);
        multiScopeData[channel][i] = level;
    }
}

void ChannelSpectrumVisualizer::drawFrame(juce::Graphics& g) {
    g.drawSingleLineText(title, 10, 20);

    int currN = State::GetDenormalizedValue("channelN");

    for (int channel = 0; channel < std::min(currN, maxN); channel++)
    {
        g.setColour(State::colours[channel]);
        g.drawSingleLineText("C" + std::to_string(channel + 1), 10+20 * (channel), 40);
        auto test = multiScopeData;

        for (int i = 1; i < scopeSize; ++i)
        {
            auto width = getLocalBounds().getWidth();
            auto height = getLocalBounds().getHeight();

            g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                                  juce::jmap(multiScopeData[channel][i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                          (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                                  juce::jmap(multiScopeData[channel][i],     0.0f, 1.0f, (float)height, 0.0f) });
        }
    }
}

void ChannelSpectrumVisualizer::setTitle(std::string str) {
    title = str;
}


void ChannelSpectrumVisualizer::pushNextSampleIntoFifoTable(int channel, float sample) noexcept
{
    if (channel >= maxN) return;
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    if (index[channel] == fftSize)
    {
        if (!blockReady[channel])
        {
            juce::zeromem(fftTable[channel], sizeof(fftTable[channel]));
            memcpy(fftTable[channel], fifoTable[channel], sizeof(fifoTable[channel]));
            blockReady[channel] = true;
        }

        index[channel] = 0;
    }

    fifoTable[channel][index[channel]++] = sample;
}