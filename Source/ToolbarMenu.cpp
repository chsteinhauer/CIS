/*
  ==============================================================================

    ToolbarMenu.cpp
    Created: 9 Nov 2022 4:01:20pm
    Author:  chris

  ==============================================================================
*/

#include "ToolbarMenu.h"

ToolbarMenu::ToolbarMenu()
	: toolbar(0)
{
	addAndMakeVisible(toolbar = new juce::Toolbar());

	auto items = new ToolbarItems();
	toolbar->addDefaultItems(*items);
}

ToolbarMenu::~ToolbarMenu()
{
	deleteAndZero(toolbar);

}

void ToolbarMenu::resized()
{
	toolbar->setSize(30, 100);
}

void ToolbarMenu::buttonClicked(juce::Button* buttonThatWasClicked)
{

}