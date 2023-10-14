#pragma once

#include <JuceHeader.h>
#include "BorderedComponent.h"

class SamplesPanel : public BorderedComponent
{
public:
    SamplesPanel();
    ~SamplesPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesPanel)
};
