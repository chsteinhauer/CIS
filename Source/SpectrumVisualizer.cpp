

#include "SpectrumVisualizer.h"

SpectrumVisualizer::SpectrumVisualizer() : forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    setOpaque(true);
    startTimerHz(20);
    setSize(400, 200);
}

SpectrumVisualizer::~SpectrumVisualizer()
{

}

void SpectrumVisualizer::paint(juce::Graphics& g) 
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).darker(0.95F));

    g.setOpacity(0.9f);
    g.setColour(juce::Colours::white);
    drawFrame(g);
}

void SpectrumVisualizer::timerCallback()
{
    if (nextFFTBlockReady)
    {
        drawNextFrameOfSpectrum();
        nextFFTBlockReady = false;
        repaint();
    }
}

void SpectrumVisualizer::drawNextFrameOfSpectrum()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable(fftData, fftSize);     
    
    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);  

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < scopeSize; ++i)                    
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
        auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
            - juce::Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);
        scopeData[i] = level;
    }

    if (showThreshold) {
        float y = -State::GetDenormalizedValue("threshold");
        threshold = juce::Line<float>(0,y,getWidth(),y);
    }
}

void SpectrumVisualizer::drawFrame(juce::Graphics& g) {
    g.drawSingleLineText(title, 10, 20);

    for (int i = 1; i < scopeSize; ++i)
    {
        auto width = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();

        g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                              juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                      (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                              juce::jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
    }

    if (showThreshold) {
        g.setColour(juce::Colours::aqua);
        g.drawLine(threshold);
    }
}

void SpectrumVisualizer::setTitle(std::string str) {
    title = str;
}

void SpectrumVisualizer::setThreshold(bool show) {
    showThreshold = show;
}

void SpectrumVisualizer::pushNextSampleIntoFifo(float sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    if (fifoIndex == fftSize)              
    {
        if (!nextFFTBlockReady)           
        {
            juce::zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;         
}