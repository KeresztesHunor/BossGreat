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
    static const int padding = 8;

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
    juce::DrawableButton toggleRecordModeButton;
    
    void setRecordButtonToggleState();
    void initDrawableCompositeForSampleSelectionButton(juce::DrawableComposite&, juce::DrawableRectangle&, juce::DrawableText&, juce::Colour);
    void initDrawableCompositeForToggleRecordModeButton(juce::DrawableComposite&, juce::Path&, juce::DrawablePath&, juce::FillType&);
    void setCurrentlySelectedSamplePanelButtonToggleState(bool);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BossGreatAudioProcessorEditor)
};
