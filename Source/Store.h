/*
  ==============================================================================

    Store.h
    Created: 1 Nov 2022 11:15:32am
    Author:  chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

static class Store {
public:
    Store(juce::AudioProcessor& processor, juce::File& file) : treeState(processor, parseFile(file)) {

    }
    

private:

    struct TreeState {

        TreeState(juce::AudioProcessor& processor, juce::var data) 
            : tree(processor, nullptr, "PARAMETERS", createParameterLayout(data["parameters"]))
        {
            // TODO: parse filters here?
        }

        juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(juce::var parameters)
        {
            juce::AudioProcessorValueTreeState::ParameterLayout layout;

            for (int i = 1; i < parameters.size(); ++i) {
                auto par = parameters[i];

                layout.add(std::make_unique<juce::AudioParameterFloat>(par["name"], par["name"], par["min"],par["max"],par["default"]));
            }
        }

    private:
        juce::AudioProcessorValueTreeState tree;

    };

    juce::var parseFile(juce::File& file) {
        return juce::JSON::parse(file);
    }

    Store::TreeState treeState;
    static int N;
    
};


