#pragma once

#include <JuceHeader.h>
#include "BorderedComponent.h"
#include "PluginProcessor.h"

class WaveFormDisplay : public BorderedComponent
{
public:
    static const int gridTemplateRowFr = 3;

    WaveFormDisplay(BossGreatAudioProcessor&);
    ~WaveFormDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    BossGreatAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveFormDisplay)
};
