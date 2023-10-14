#pragma once

#include <JuceHeader.h>
#include "BorderedComponent.h"

class SpectrumView : public BorderedComponent
{
public:
    static const int gridTemplateRowFr { 5 };

    SpectrumView();
    ~SpectrumView() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumView)
};
