#pragma once

#include <JuceHeader.h>

class BossGreatAudioProcessor : public juce::AudioProcessor
                             #if JucePlugin_Enable_ARA
                              , public juce::AudioProcessorARAExtension
                             #endif
{
public:
    std::function<void()> recordModeStateChangedEvent;
    std::function<void()> recordingBufferUpdatedEvent;

    BossGreatAudioProcessor();
    ~BossGreatAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void toggleRecordMode();

    bool getRecordModeIsOn();

    juce::AudioBuffer<float>& getSample(int samplePanelIndex);

private:

    static const int numSamplesToStore { 16 };
    int numSamplesToRecord { 44100 };
    int selectedSamplePanelIndex;
    int recordingBufferSampleIndex;

    double sampleRate { 44100 };

    bool recordModeIsOn;

    juce::AudioBuffer<float> samples[numSamplesToStore];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BossGreatAudioProcessor)
};
