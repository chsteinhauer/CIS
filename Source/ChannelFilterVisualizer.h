

#pragma once
#include <JuceHeader.h>
#include "SimulationState.h"

class ChannelFilterVisualizer : public juce::Component {
public:
    ChannelFilterVisualizer();
    ~ChannelFilterVisualizer();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void addChannelFilter(int channel, double* _frequencies, double* _magnitudes);

private:
    std::vector<double*> frequencies;
    std::vector<double*> magnitudes;
    int size = 0;
};