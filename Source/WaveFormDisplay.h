#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class WaveFormDisplay : public juce::Component
{
public:
    WaveFormDisplay(BossGreatAudioProcessor&);
    ~WaveFormDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    BossGreatAudioProcessor& audioProcessor;
};
