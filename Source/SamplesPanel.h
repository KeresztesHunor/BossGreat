#pragma once

#include <JuceHeader.h>

class SamplesPanel : public juce::Component
{
public:
    SamplesPanel();
    ~SamplesPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesPanel)
};
