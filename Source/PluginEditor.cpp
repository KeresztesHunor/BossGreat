#include "MainColours.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessorEditor::BossGreatAudioProcessorEditor(BossGreatAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), waveformDisplay(p)
{
    // Set plugin size

    setSize (640, 360);

    //

    const int halfWidth = getWidth() / 2;
    const int halfOfNumSamplesToStore = BossGreatAudioProcessor::numSamplesToStore / 2;
    const int totalPaddingThickness = padding * 2;
    
    // Create the samples panel

    int samplesPanelWidth = halfWidth - padding;
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
    juce::DrawableRectangle sampleSelectionButtonDrawable;
    sampleSelectionButtonDrawable.setRectangle(juce::Parallelogram<float>(juce::Rectangle<float>(buttonWidth, buttonHeight)));
    sampleSelectionButtonDrawable.setFill(juce::FillType(MainColours::colour1));
    sampleSelectionButtonDrawable.setStrokeFill(juce::FillType(juce::Colours::white));
    sampleSelectionButtonDrawable.setStrokeThickness(1.f);
    for (int i = 0; i < halfOfNumSamplesToStore; i++)
    {
        initSampleSelectionButton(i, halfOfNumSamplesToStore, buttonWidth, buttonHeight, &sampleSelectionButtonDrawable);
        initSampleSelectionButton(i + halfOfNumSamplesToStore, halfOfNumSamplesToStore, buttonWidth, buttonHeight, &sampleSelectionButtonDrawable);
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
    const std::string state = audioProcessor.getRecordModeIsOn() ? "on" : "off";
    toggleRecordModeButton.setButtonText("Record mode: " + state);
}

void BossGreatAudioProcessorEditor::initSampleSelectionButton(int index, int halfOfNumSamplesToStore, int buttonWidth, int buttonHeight, juce::DrawableRectangle* sampleSelectionButtonDrawable)
{
    sampleSelectionButtons[index] = new juce::DrawableButton("Sample selection button " + static_cast<juce::String>(index), juce::DrawableButton::ButtonStyle::ImageRaw);
    juce::DrawableButton& currentButton = *sampleSelectionButtons[index];
    currentButton.setImages(sampleSelectionButtonDrawable);
    currentButton.setBounds((index % halfOfNumSamplesToStore) * buttonWidth, (index / halfOfNumSamplesToStore) * buttonHeight, buttonWidth, buttonHeight);
    sampleSelectionButtonsView.addAndMakeVisible(currentButton);
}
