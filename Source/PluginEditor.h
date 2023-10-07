#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BossGreatAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    BossGreatAudioProcessorEditor(BossGreatAudioProcessor&);
    ~BossGreatAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    class WaveformDisplay : public juce::Component
    {
    public:
        WaveformDisplay(BossGreatAudioProcessor& audioProcessor);
        ~WaveformDisplay();

        void paint(juce::Graphics& g) override;

    private:
        BossGreatAudioProcessor& audioProcessor;
    };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BossGreatAudioProcessor& audioProcessor;

    juce::TextButton toggleRecordModeButton;

    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BossGreatAudioProcessorEditor)
};
