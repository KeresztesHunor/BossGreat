#pragma once

#include <JuceHeader.h>

class BorderedComponent : public juce::Component
{
public:
    BorderedComponent(int, juce::Colour);
    ~BorderedComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    int getWidthWithoutBorder();
    int getHeightWithoutBorder();

    int getBorderThickness();

private:
    int borderThickness;

    juce::Colour borderColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BorderedComponent)
};
