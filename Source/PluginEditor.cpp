#include "MainColours.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessorEditor::BossGreatAudioProcessorEditor(BossGreatAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), waveformDisplay(p)
{
    // Set plugin size

    setSize (640, 360);

    //

    const int halfWidth = getWidth() / 2;
    
    // Create the samples panel

    samplesPanel.setBounds(padding, padding, halfWidth - padding, getHeight() - padding * 2);

    //

    const int samplesPanelSegmentHeight = samplesPanel.getHeightWithoutBorder() / (SampleSelectionButtons::gridTemplateRowFr + WaveFormDisplay::gridTemplateRowFr + SpectrumView::gridTemplateRowFr);

    // Create the sample selection buttons panel

    sampleSelectionButtons.setBounds(samplesPanel.getBorderThickness(), samplesPanel.getBorderThickness(), samplesPanel.getWidthWithoutBorder(), samplesPanelSegmentHeight * SampleSelectionButtons::gridTemplateRowFr);
    samplesPanel.addAndMakeVisible(sampleSelectionButtons);

    // Create the sample selection buttons

    // Create the waveform display

    waveformDisplay.setBounds(samplesPanel.getBorderThickness(), samplesPanel.getBorderThickness() + sampleSelectionButtons.getHeight(), samplesPanel.getWidthWithoutBorder(), samplesPanelSegmentHeight * WaveFormDisplay::gridTemplateRowFr);
    samplesPanel.addAndMakeVisible(waveformDisplay);
    audioProcessor.recordingBufferUpdatedEvent = [&]() {
        waveformDisplay.repaint();
    };
    addAndMakeVisible(samplesPanel);

    // Create the spectrum view panel

    const int spectrumViewYPos = sampleSelectionButtons.getHeight() + waveformDisplay.getHeight();
    spectrumView.setBounds(samplesPanel.getBorderThickness(), samplesPanel.getBorderThickness() + spectrumViewYPos, samplesPanel.getWidthWithoutBorder(), samplesPanel.getHeightWithoutBorder() - spectrumViewYPos);
    samplesPanel.addAndMakeVisible(spectrumView);

    // Create the controls panel

    controlsPanel.setBounds(halfWidth, padding, getWidth() - halfWidth - padding, getHeight() - padding * 2);
    addAndMakeVisible(controlsPanel);

    // Create a text button on the controls panel

    toggleRecordModeButton.setBounds(8, 8, 128, 16);
    setRecordButtonText();
    controlsPanel.addAndMakeVisible(toggleRecordModeButton);
    toggleRecordModeButton.onClick = [&]() {
        audioProcessor.toggleRecordMode();
    };

    //

    audioProcessor.recordModeStateChangedEvent = [&]() {
        setRecordButtonText();
        if (!audioProcessor.getRecordModeIsOn())
        {
            audioProcessor.getSelectedSamplePanel().processBuffers();
            waveformDisplay.repaint();
        }
    };
}

BossGreatAudioProcessorEditor::~BossGreatAudioProcessorEditor()
{
    if (audioProcessor.getRecordModeIsOn())
    {
        audioProcessor.toggleRecordMode();
    }
}

void BossGreatAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(MainColours::colour1darker);
}

void BossGreatAudioProcessorEditor::resized()
{
    
}

void BossGreatAudioProcessorEditor::setRecordButtonText()
{
    const std::string state = audioProcessor.getRecordModeIsOn() ? "on" : "off";
    toggleRecordModeButton.setButtonText("Record mode: " + state);
}
