
#pragma once
#include <JuceHeader.h>

class ToolbarItems : public juce::ToolbarItemFactory
{
public:
    ToolbarItems() {}
    ~ToolbarItems() {}

    enum ToolbarItemIds
    {
        settingsButton = 1,
        volumeButton = 2,
    };

    void getAllToolbarItemIds(juce::Array <int>& ids)
    {
        ids.add(settingsButton);
        ids.add(volumeButton);
        ids.add(separatorBarId);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
    }

    void getDefaultItemSet(juce::Array <int>& ids)
    {
        ids.add(separatorBarId);
        ids.add(flexibleSpacerId);
        ids.add(flexibleSpacerId);
        ids.add(separatorBarId);
        ids.add(settingsButton);
        ids.add(separatorBarId);
        ids.add(volumeButton);
        ids.add(separatorBarId);
    }

    juce::ToolbarItemComponent* createItem(const int itemId)
    {

        switch (itemId)
        {
        case settingsButton:
            return new juce::ToolbarButton(itemId, "Settings", juce::Drawable::createFromImageData(BinaryData::settings24_png, BinaryData::settings24_pngSize), 0);

        case volumeButton:
            return new juce::ToolbarButton(itemId, "Volume", juce::Drawable::createFromImageData(BinaryData::settings24_png, BinaryData::settings24_pngSize), 0);

        default:
            break;
        }

        return 0;
    }
private:

    

};