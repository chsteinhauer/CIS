/*
  ==============================================================================

    ToolbarMenu.h
    Created: 9 Nov 2022 4:01:20pm
    Author:  chris

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ToolbarItems.h"

class ToolbarMenu : public juce::Component,
	public juce::Button::Listener
{
public:
	ToolbarMenu();
	~ToolbarMenu();

	void resized();
	void buttonClicked(juce::Button* buttonThatWasClicked);

private:
	juce::Toolbar* toolbar;
};
