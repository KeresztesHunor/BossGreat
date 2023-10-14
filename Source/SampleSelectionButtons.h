#pragma once

#include <JuceHeader.h>

class SampleSelectionButtons : public juce::Component
{
public:
    static const int gridTemplateRowFr { 1 };

    SampleSelectionButtons();
    ~SampleSelectionButtons() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleSelectionButtons)
};
