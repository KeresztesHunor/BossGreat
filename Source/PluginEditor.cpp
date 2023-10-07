#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessorEditor::BossGreatAudioProcessorEditor(BossGreatAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), waveformDisplay(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (512, 288);
    toggleRecordModeButton.setName("toggleRecordModeButton");
    toggleRecordModeButton.setButtonText("Record mode: off");
    toggleRecordModeButton.setBounds(8, 8, 128, 16);
    toggleRecordModeButton.onClick = [&]() {
        audioProcessor.toggleRecordMode();
    };
    audioProcessor.recordModeStateChangedEvent = [&]() {
        std::string state = audioProcessor.getRecordModeIsOn() ? "on" : "off";
        toggleRecordModeButton.setButtonText("Record mode: " + state);
    };
    addAndMakeVisible(toggleRecordModeButton);
    waveformDisplay.setBounds(0, 32, getWidth(), getHeight() - 32);
    audioProcessor.recordingBufferUpdatedEvent = [&]() {
        waveformDisplay.repaint();
    };
    addAndMakeVisible(waveformDisplay);
}

BossGreatAudioProcessorEditor::~BossGreatAudioProcessorEditor()
{

}

void BossGreatAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
}

void BossGreatAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

BossGreatAudioProcessorEditor::WaveformDisplay::WaveformDisplay(BossGreatAudioProcessor& audioProcessor) : juce::Component("Waveform display"), audioProcessor(audioProcessor)
{

}

BossGreatAudioProcessorEditor::WaveformDisplay::~WaveformDisplay()
{

}

void BossGreatAudioProcessorEditor::WaveformDisplay::paint(juce::Graphics& g)
{
    juce::AudioBuffer<float>& buffer = audioProcessor.getSample(0);
    if (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0)
    {
        g.setColour(juce::Colours::white);
        float sampleChunkSize = buffer.getNumSamples() / static_cast<float>(getWidth());
        int roundedChunkBorderLoewerIndex = 0;
        for (int pixelIndex = 0; pixelIndex < getWidth(); pixelIndex++)
        {
            int roundedChunkBorderUpperIndex = static_cast<int>(std::round(sampleChunkSize * (pixelIndex + 1)));
            float minValue = buffer.getSample(0, roundedChunkBorderLoewerIndex);
            float maxValue = minValue;
            for (int sampleIndex = roundedChunkBorderLoewerIndex + 1; sampleIndex < roundedChunkBorderUpperIndex; sampleIndex++)
            {
                float sampleValue = buffer.getSample(0, sampleIndex);
                if (sampleValue < minValue)
                {
                    minValue = sampleValue;
                }
                if (sampleValue > maxValue)
                {
                    maxValue = sampleValue;
                }
            }
            roundedChunkBorderLoewerIndex = roundedChunkBorderUpperIndex;
            float minY = juce::jmap<float>(minValue, -1.f, 1.f, getHeight(), 0.f);
            float maxY = juce::jmap<float>(maxValue, -1.f, 1.f, getHeight(), 0.f);
            g.drawLine(pixelIndex, maxY, pixelIndex, minY);
        }
    }
}
