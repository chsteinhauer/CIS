

#include "MenuBar.h"

MenuBar::MenuBar() {}
MenuBar::~MenuBar() {}

// If item should be placed in the upper section of the menu bar
void MenuBar::addItemUp(juce::Component& item) {
    addAndMakeVisible(item);
    itemsUp.add(&item);
}

// If item should be placed in the lower section of the menu bar
void MenuBar::addItemDown(juce::Component& item) {
    addAndMakeVisible(item);
    itemsDown.add(&item);
}

void MenuBar::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId).brighter());
}

void MenuBar::resized() {
    juce::FlexBox menu;
    menu.alignContent = juce::FlexBox::AlignContent::center;
    menu.flexDirection = juce::FlexBox::Direction::column;

    juce::FlexBox up;
    menu.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    up.alignContent = juce::FlexBox::AlignContent::center;
    up.flexDirection = juce::FlexBox::Direction::column;


    for (int i = 0; i < itemsUp.size(); i++) {
        auto item = itemsUp[i];
        up.items.add(juce::FlexItem(*item)
            .withMinHeight(item->getWidth())
            .withMinWidth(item->getHeight())
            .withMargin({ 7,7,0,7 }));
    }

    juce::FlexBox down;
    down.alignContent = juce::FlexBox::AlignContent::center;
    down.flexDirection = juce::FlexBox::Direction::column;
    menu.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    for (int i = 0; i < itemsDown.size(); i++) {
        auto item = itemsDown[i];
        down.items.add(juce::FlexItem(*item)
            .withMinHeight(item->getHeight())
            .withMinWidth(item->getWidth())
            .withMargin({ 0,7,7,7 }));
    }


    menu.items.add(juce::FlexItem(up).withFlex(1).withMargin({ 4,0,0,0 }));
    menu.items.add(juce::FlexItem(down).withMinHeight(39 * itemsDown.size()));

    menu.performLayout(getLocalBounds().toFloat());
}