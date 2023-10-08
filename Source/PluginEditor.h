#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveFormDisplay.h"
#include "SamplesPanel.h"
#include "ControlsPanel.h"

class BossGreatAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    BossGreatAudioProcessorEditor(BossGreatAudioProcessor&);
    ~BossGreatAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BossGreatAudioProcessor& audioProcessor;
    WaveFormDisplay waveformDisplay;
    SamplesPanel samplesPanel;
    ControlsPanel controlsPanel;
    juce::TextButton toggleRecordModeButton;

    void setRecordButtonText();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BossGreatAudioProcessorEditor)
};