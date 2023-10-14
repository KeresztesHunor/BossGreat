#pragma once

#include <JuceHeader.h>
#include "BorderedComponent.h"

class ControlsPanel : public BorderedComponent
{
public:
    ControlsPanel();
    ~ControlsPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlsPanel)
};
