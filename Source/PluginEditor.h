#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SamplesPanel.h"
#include "SampleSelectionButtonsView.h"
#include "WaveFormDisplay.h"
#include "SpectrumView.h"
#include "ControlsPanel.h"

class BossGreatAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    static const int padding { 8 };

    BossGreatAudioProcessorEditor(BossGreatAudioProcessor&);
    ~BossGreatAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BossGreatAudioProcessor& audioProcessor;
    SamplesPanel samplesPanel;
    SampleSelectionButtonsView sampleSelectionButtonsView;
    juce::DrawableButton* sampleSelectionButtons[BossGreatAudioProcessor::numSamplesToStore];
    WaveFormDisplay waveformDisplay;
    SpectrumView spectrumView;
    ControlsPanel controlsPanel;
    juce::TextButton toggleRecordModeButton;
    
    void setRecordButtonText();
    void initDrawableRectangleForSampleSelectionButton(juce::DrawableRectangle&, juce::Colour, int, int);
    void initSampleSelectionButton(int, int, int, int, juce::DrawableRectangle*, juce::DrawableRectangle*, juce::DrawableRectangle*);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BossGreatAudioProcessorEditor)
};
