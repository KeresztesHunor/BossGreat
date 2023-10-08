#include "MainColours.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessorEditor::BossGreatAudioProcessorEditor(BossGreatAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), waveformDisplay(p)
{
    // Set plugin size

    setSize (640, 360);

    // Get half the width of the plugin window

    const int halfWidth = getWidth() / 2;

    // Create the samples panel

    samplesPanel.setBounds(0, 0, halfWidth, getHeight());
    addAndMakeVisible(samplesPanel);

    // Create the controls panel

    controlsPanel.setBounds(halfWidth, 0, getWidth() - halfWidth, getHeight());
    addAndMakeVisible(controlsPanel);

    // Create a text button on the controls panel

    toggleRecordModeButton.setBounds(8, 8, 128, 16);
    setRecordButtonText();
    controlsPanel.addAndMakeVisible(toggleRecordModeButton);
    toggleRecordModeButton.onClick = [&]() {
        audioProcessor.toggleRecordMode();
    };

    // Create the waveform display

    waveformDisplay.setBounds(0, 0, samplesPanel.getWidth(), samplesPanel.getHeight());
    samplesPanel.addAndMakeVisible(waveformDisplay);
    audioProcessor.recordingBufferUpdatedEvent = [&]() {
        waveformDisplay.repaint();
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
    g.fillAll(MainColours::colour1);
}

void BossGreatAudioProcessorEditor::resized()
{
    
}

void BossGreatAudioProcessorEditor::setRecordButtonText()
{
    const std::string state = audioProcessor.getRecordModeIsOn() ? "on" : "off";
    toggleRecordModeButton.setButtonText("Record mode: " + state);
}
