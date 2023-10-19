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
    for (int i = 0; i < BossGreatAudioProcessor::numSamplesToStore; i++)
    {
        const juce::String buttonNumberText = static_cast<juce::String>(i + 1);
        sampleSelectionButtons[i] = new juce::DrawableButton("Sample selection button " + buttonNumberText, juce::DrawableButton::ButtonStyle::ImageStretched);
        juce::DrawableButton& currentButton = *sampleSelectionButtons[i];

        // These need to be allocated to the heap because when the code
        // steps out of the for loop's scope, it calls the DrawableComposite's
        // destructor which attempts to free up all its child components' memory

        juce::Parallelogram parallelogram(juce::Rectangle<float>(buttonWidth, buttonHeight));

        juce::DrawableRectangle* normalImageRectangle = new juce::DrawableRectangle;
        normalImageRectangle->setRectangle(parallelogram);
        normalImageRectangle->setFill(MainColours::colour1);
        normalImageRectangle->setStrokeFill(juce::FillType(juce::Colours::white));
        normalImageRectangle->setStrokeThickness(1.f);
        juce::DrawableRectangle* overImageRectangle = initDrawableRectangleForSampleSelectionButton(normalImageRectangle, MainColours::colour1light);
        juce::DrawableRectangle* downImageRectangle = initDrawableRectangleForSampleSelectionButton(normalImageRectangle, MainColours::colour1lighter);
        juce::DrawableRectangle* normalImageOnRectangle = initDrawableRectangleForSampleSelectionButton(normalImageRectangle, MainColours::colour1light);
        juce::DrawableRectangle* overImageOnRectangle = initDrawableRectangleForSampleSelectionButton(normalImageRectangle, MainColours::colour1lighter);

        juce::DrawableText* normalImageText = new juce::DrawableText;
        normalImageText->setBoundingBox(parallelogram);
        normalImageText->setFont(juce::Font(buttonHeight), true);
        normalImageText->setJustification(juce::Justification(juce::Justification::centred));
        normalImageText->setColour(juce::Colours::white);
        normalImageText->setText(buttonNumberText);
        juce::DrawableText* overImageText = new juce::DrawableText(*normalImageText);
        juce::DrawableText* downImageText = new juce::DrawableText(*normalImageText);
        juce::DrawableText* normalImageOnText = new juce::DrawableText(*normalImageText);
        juce::DrawableText* overImageOnText = new juce::DrawableText(*normalImageText);

        juce::DrawableComposite normalImage;
        normalImage.addAndMakeVisible(normalImageRectangle);
        normalImage.addAndMakeVisible(normalImageText);
        juce::DrawableComposite overImage;
        overImage.addAndMakeVisible(overImageRectangle);
        overImage.addAndMakeVisible(overImageText);
        juce::DrawableComposite downImage;
        downImage.addAndMakeVisible(downImageRectangle);
        downImage.addAndMakeVisible(downImageText);
        juce::DrawableComposite normalImageOn;
        normalImageOn.addAndMakeVisible(normalImageOnRectangle);
        normalImageOn.addAndMakeVisible(normalImageOnText);
        juce::DrawableComposite overImageOn;
        overImageOn.addAndMakeVisible(overImageOnRectangle);
        overImageOn.addAndMakeVisible(overImageOnText);

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

juce::DrawableRectangle* BossGreatAudioProcessorEditor::initDrawableRectangleForSampleSelectionButton(juce::DrawableRectangle* copyable, juce::Colour colour)
{
    juce::DrawableRectangle* drawable = new juce::DrawableRectangle(*copyable);
    drawable->setFill(juce::FillType(colour));
    return drawable;
}
