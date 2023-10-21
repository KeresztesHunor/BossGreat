#include "MainColours.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessorEditor::BossGreatAudioProcessorEditor(BossGreatAudioProcessor& p) : AudioProcessorEditor(&p)
                                                                                         , audioProcessor(p)
                                                                                         , waveformDisplay(p)
                                                                                         , toggleRecordModeButton("Toggle record mode button", juce::DrawableButton::ButtonStyle::ImageStretched)
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
    const juce::Parallelogram parallelogram(juce::Rectangle<float>(buttonWidth, buttonHeight));
    juce::DrawableRectangle copyableDrawableRectangle;
    copyableDrawableRectangle.setRectangle(parallelogram);
    copyableDrawableRectangle.setStrokeFill(juce::FillType(juce::Colours::white));
    copyableDrawableRectangle.setStrokeThickness(1.f);
    juce::DrawableText copyableDrawableText;
    copyableDrawableText.setBoundingBox(parallelogram);
    copyableDrawableText.setFont(juce::Font("Courier New", buttonHeight, juce::Font::FontStyleFlags::bold), true);
    copyableDrawableText.setJustification(juce::Justification(juce::Justification::centred));
    copyableDrawableText.setColour(juce::Colours::white);
    for (int i = 0; i < BossGreatAudioProcessor::numSamplesToStore; i++)
    {
        const juce::String buttonNumberText = static_cast<juce::String>(i + 1);
        sampleSelectionButtons[i] = new juce::DrawableButton("Sample selection button " + buttonNumberText, juce::DrawableButton::ButtonStyle::ImageStretched);
        juce::DrawableButton& currentButton = *sampleSelectionButtons[i];
        copyableDrawableText.setText(buttonNumberText);
        juce::DrawableComposite normalImage;
        initDrawableCompositeForSampleSelectionButton(normalImage, copyableDrawableRectangle, copyableDrawableText, MainColours::colour1);
        juce::DrawableComposite overImage;
        initDrawableCompositeForSampleSelectionButton(overImage, copyableDrawableRectangle, copyableDrawableText, MainColours::colour1light);
        juce::DrawableComposite downImage;
        initDrawableCompositeForSampleSelectionButton(downImage, copyableDrawableRectangle, copyableDrawableText, MainColours::colour1lighter);
        juce::DrawableComposite normalImageOn;
        initDrawableCompositeForSampleSelectionButton(normalImageOn, copyableDrawableRectangle, copyableDrawableText, MainColours::colour1light);
        juce::DrawableComposite overImageOn;
        initDrawableCompositeForSampleSelectionButton(overImageOn, copyableDrawableRectangle, copyableDrawableText, MainColours::colour1light);
        currentButton.setImages(&normalImage, &overImage, &downImage, (const juce::Drawable*)nullptr, &normalImageOn, &overImageOn);
        currentButton.setBounds((i % halfOfNumSamplesToStore) * buttonWidth, (i / halfOfNumSamplesToStore) * buttonHeight, buttonWidth, buttonHeight);
        sampleSelectionButtonsView.addAndMakeVisible(currentButton);
        currentButton.onClick = [this, i]() {
            if (audioProcessor.getSelectedSampleDataIndex() != i)
            {
                if (audioProcessor.getRecordModeIsOn())
                {
                    audioProcessor.toggleRecordMode();
                }
                setCurrentlySelectedSamplePanelButtonToggleState(false);
                audioProcessor.selectSampleData(i);
                setCurrentlySelectedSamplePanelButtonToggleState(true);
            }
        };
    }
    setCurrentlySelectedSamplePanelButtonToggleState(true);

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

    toggleRecordModeButton.setBounds(8, 8, 32, 32);
    const int recordButtonHalfWidth = toggleRecordModeButton.getWidth() / 2;
    const int recordButtonHalfHeight = toggleRecordModeButton.getHeight() / 2;
    int redCircleWidth = static_cast<int>(std::round(toggleRecordModeButton.getWidth() * 0.42f));
    int redCircleHeight = static_cast<int>(std::round(toggleRecordModeButton.getHeight() * 0.42f));
    if (redCircleWidth % 2 != 0)
    {
        redCircleWidth--;
    }
    if (redCircleHeight % 2 != 0)
    {
        redCircleHeight--;
    }
    juce::Path recordButtonOutline;
    recordButtonOutline.addEllipse(0, 0, toggleRecordModeButton.getWidth(), toggleRecordModeButton.getHeight());
    juce::Path recordButtonRedCircle;
    recordButtonRedCircle.addEllipse(recordButtonHalfWidth - redCircleWidth / 2, recordButtonHalfHeight - redCircleHeight / 2, redCircleWidth, redCircleHeight);
    juce::DrawablePath recordButtonImageRedCircle;
    recordButtonImageRedCircle.setFill(juce::FillType(juce::Colours::red));
    recordButtonImageRedCircle.setPath(recordButtonRedCircle);
    juce::DrawableComposite recordButtonNormalImage;
    initDrawableCompositeForToggleRecordModeButton(recordButtonNormalImage, recordButtonOutline, recordButtonImageRedCircle, juce::FillType(juce::ColourGradient(juce::Colours::transparentBlack, recordButtonHalfWidth, recordButtonHalfHeight, juce::Colours::black, 0, 0, true)));
    juce::DrawableComposite recordButtonNormalImageOn;
    initDrawableCompositeForToggleRecordModeButton(recordButtonNormalImageOn, recordButtonOutline, recordButtonImageRedCircle, juce::FillType(juce::ColourGradient(juce::Colours::white, recordButtonHalfWidth, recordButtonHalfHeight, juce::Colour(0.f, 1.f, 0.75f, 1.f), 0, 0, true)));
    toggleRecordModeButton.setImages(&recordButtonNormalImage, (const juce::Drawable*)nullptr, (const juce::Drawable*)nullptr, (const juce::Drawable*)nullptr, &recordButtonNormalImageOn);
    controlsPanel.addAndMakeVisible(toggleRecordModeButton);
    setRecordButtonToggleState();
    toggleRecordModeButton.onClick = [&]() {
        audioProcessor.toggleRecordMode();
    };

    //

    audioProcessor.recordModeStateChangedEvent = [&]() {
        setRecordButtonToggleState();
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

void BossGreatAudioProcessorEditor::setRecordButtonToggleState()
{
    toggleRecordModeButton.setToggleState(audioProcessor.getRecordModeIsOn(), juce::NotificationType::dontSendNotification);
}

void BossGreatAudioProcessorEditor::initDrawableCompositeForSampleSelectionButton(juce::DrawableComposite& drawable, juce::DrawableRectangle& copyableDrawableRectangle, juce::DrawableText& copyableDrawableText, juce::Colour colour)
{
    juce::DrawableRectangle* drawableRectangle = new juce::DrawableRectangle(copyableDrawableRectangle);
    drawableRectangle->setFill(juce::FillType(colour));
    drawable.addAndMakeVisible(drawableRectangle);
    drawable.addAndMakeVisible(new juce::DrawableText(copyableDrawableText));
}

void BossGreatAudioProcessorEditor::initDrawableCompositeForToggleRecordModeButton(juce::DrawableComposite& drawable, juce::Path& outline, juce::DrawablePath& redCircle, juce::FillType& fill)
{
    juce::DrawablePath* recordButtonImageBackground = new juce::DrawablePath();
    recordButtonImageBackground->setStrokeFill(juce::FillType(juce::Colours::white));
    recordButtonImageBackground->setStrokeThickness(1.f);
    recordButtonImageBackground->setPath(outline);
    recordButtonImageBackground->setFill(fill);
    drawable.addAndMakeVisible(recordButtonImageBackground);
    drawable.addAndMakeVisible(new juce::DrawablePath(redCircle));
}

void BossGreatAudioProcessorEditor::setCurrentlySelectedSamplePanelButtonToggleState(bool state)
{
    sampleSelectionButtons[audioProcessor.getSelectedSampleDataIndex()]->setToggleState(state, juce::NotificationType::dontSendNotification);
}
