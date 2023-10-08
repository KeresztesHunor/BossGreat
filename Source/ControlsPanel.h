#pragma once

#include <JuceHeader.h>

class ControlsPanel : public juce::Component
{
public:
    ControlsPanel();
    ~ControlsPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlsPanel)
};
