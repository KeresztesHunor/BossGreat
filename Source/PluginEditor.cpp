#include "MainColours.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessorEditor::BossGreatAudioProcessorEditor(BossGreatAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), waveformDisplay(p)
{
    // Set plugin size

    setSize (640, 360);

    //

    const int halfOfNumSamplesToStore = BossGreatAudioProcessor::numSamplesToStore / 2;
    const int totalPaddingThickness = padding * 2;
    
    // Create the samples panel

    int samplesPanelWidth = getWidth() / 2 - padding;
    const int samplesPanelWidthDivisionRemainder = (samplesPanelWidth - samplesPanel.getBorderThickness() * 2) % halfOfNumSamplesToStore;
    if (samplesPanelWidthDivisionRemainder > 0)
    {
        samplesPanelWidth -= samplesPanelWidthDivisionRemainder <= halfOfNumSamplesToStore / 2 ? samplesPanelWidthDivisionRemainder : samplesPanelWidthDivisionRemainder - halfOfNumSamplesToStore;
    }
    samplesPanel.setBounds(padding, padding, samplesPanelWidth, getHeight() - totalPaddingThickness);
    addAndMakeVisible(samplesPanel);

    //

    const float samplesPanelSegmentHeight = samplesPanel.getHeightWithoutBorder() / static_cast<float>(SampleSelectionButtonsView::gridTemplateRowFr + WaveFormDisplay::gridTemplateRowFr + SpectrumView::gridTemplateRowFr);

    // Create the sample selection buttons panel

    int sampleSelectionButtonsHeight = static_cast<int>(samplesPanelSegmentHeight * SampleSelectionButtonsView::gridTemplateRowFr);
    if (sampleSelectionButtonsHeight % 2 == 1)
    {
        sampleSelectionButtonsHeight++;
    }
    sampleSelectionButtonsView.setBounds(samplesPanel.getBorderThickness(), samplesPanel.getBorderThickness(), samplesPanel.getWidthWithoutBorder(), sampleSelectionButtonsHeight);
    samplesPanel.addAndMakeVisible(sampleSelectionButtonsView);

    // Create the sample selection buttons

    const int buttonWidth = sampleSelectionButtonsView.getWidth() / halfOfNumSamplesToStore;
    const int buttonHeight = sampleSelectionButtonsView.getHeight() / 2;
    juce::DrawableRectangle normalImage;
    initDrawableRectangleForSampleSelectionButton(normalImage, buttonWidth, buttonHeight, MainColours::colour1);
    juce::DrawableRectangle overImage;
    initDrawableRectangleForSampleSelectionButton(overImage, buttonWidth, buttonHeight, MainColours::colour1light);
    juce::DrawableRectangle downImage;
    initDrawableRectangleForSampleSelectionButton(downImage, buttonWidth, buttonHeight, MainColours::colour1lighter);
    juce::DrawableRectangle normalImageOn;
    initDrawableRectangleForSampleSelectionButton(normalImageOn, buttonWidth, buttonHeight, MainColours::colour1light);
    juce::DrawableRectangle overImageOn;
    initDrawableRectangleForSampleSelectionButton(overImageOn, buttonWidth, buttonHeight, MainColours::colour1lighter);
    for (int i = 0; i < BossGreatAudioProcessor::numSamplesToStore; i++)
    {
        sampleSelectionButtons[i] = new juce::DrawableButton("Sample selection button " + static_cast<juce::String>(i), juce::DrawableButton::ButtonStyle::ImageStretched);
        juce::DrawableButton& currentButton = *sampleSelectionButtons[i];
        currentButton.setImages(&normalImage, &overImage, &downImage, nullptr, &normalImageOn, &overImageOn);
        currentButton.setBounds((i % halfOfNumSamplesToStore) * buttonWidth, (i / halfOfNumSamplesToStore) * buttonHeight, buttonWidth, buttonHeight);
        sampleSelectionButtonsView.addAndMakeVisible(currentButton);
    }

    // Create the waveform display

    int waveformDisplayHeight = static_cast<int>(samplesPanelSegmentHeight * WaveFormDisplay::gridTemplateRowFr);
    if (waveformDisplayHeight % 2 == 1)
    {
        waveformDisplayHeight++;
    }
    waveformDisplay.setBounds(samplesPanel.getBorderThickness(), samplesPanel.getBorderThickness() + sampleSelectionButtonsView.getHeight(), samplesPanel.getWidthWithoutBorder(), waveformDisplayHeight);
    samplesPanel.addAndMakeVisible(waveformDisplay);
    audioProcessor.recordingBufferUpdatedEvent = [&]() {
        waveformDisplay.repaint();
    };
    addAndMakeVisible(samplesPanel);

    // Create the spectrum view panel

    const int spectrumViewYPos = sampleSelectionButtonsView.getHeight() + waveformDisplay.getHeight();
    spectrumView.setBounds(samplesPanel.getBorderThickness(), samplesPanel.getBorderThickness() + spectrumViewYPos, samplesPanel.getWidthWithoutBorder(), samplesPanel.getHeightWithoutBorder() - spectrumViewYPos);
    samplesPanel.addAndMakeVisible(spectrumView);

    // Create the controls panel
    
    controlsPanel.setBounds(samplesPanel.getWidth() + padding, padding, getWidth() - samplesPanelWidth - totalPaddingThickness, getHeight() - totalPaddingThickness);
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
    for each (juce::DrawableButton* button in sampleSelectionButtons)
    {
        delete button;
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
    const juce::String state = audioProcessor.getRecordModeIsOn() ? "on" : "off";
    toggleRecordModeButton.setButtonText("Record mode: " + state);
}

void BossGreatAudioProcessorEditor::initDrawableRectangleForSampleSelectionButton(juce::DrawableRectangle& drawable, int buttonWidth, int buttonHeight, juce::Colour colour)
{
    drawable.setRectangle(juce::Parallelogram<float>(juce::Rectangle<float>(buttonWidth, buttonHeight)));
    drawable.setFill(juce::FillType(colour));
    drawable.setStrokeFill(juce::FillType(juce::Colours::white));
    drawable.setStrokeThickness(1.f);
}
