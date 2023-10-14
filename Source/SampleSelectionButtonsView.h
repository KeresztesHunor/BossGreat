#pragma once

#include <JuceHeader.h>

class SampleSelectionButtonsView : public juce::Component
{
public:
    static const int gridTemplateRowFr { 1 };

    SampleSelectionButtonsView();
    ~SampleSelectionButtonsView() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleSelectionButtonsView)
};
