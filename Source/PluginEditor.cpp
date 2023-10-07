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
    juce::AudioBuffer<float> scaledBuffer;
    scaledBuffer.setSize(2, getWidth());
    int sampleToPixelRatio = buffer.getNumSamples() / getWidth();
    for (int sampleIndex = 0; sampleIndex < getWidth(); sampleIndex++)
    {
        for (int channelIndex = 0; channelIndex < scaledBuffer.getNumChannels(); channelIndex++)
        {
            scaledBuffer.setSample(channelIndex, sampleIndex, channelIndex < buffer.getNumChannels() ? buffer.getSample(channelIndex, sampleIndex * sampleToPixelRatio) : 0.f);
        }
    }
    juce::Path path;
    int halfHeight = getHeight() / 2;
    path.startNewSubPath(0, halfHeight);
    for (int i = 0; i < scaledBuffer.getNumSamples(); i++)
    {
        path.lineTo(i, juce::jmap<float>(scaledBuffer.getSample(0, i), -1.f, 1.f, getHeight(), 0)); // It only does the left channel for now (channel index: 0)
    }
    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType(1.f));
}
