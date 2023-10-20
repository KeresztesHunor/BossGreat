#include "PluginProcessor.h"
#include "PluginEditor.h"

BossGreatAudioProcessor::BossGreatAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                   #if ! JucePlugin_IsMidiEffect
                    #if ! JucePlugin_IsSynth
                     .withInput("Input",  juce::AudioChannelSet::stereo(), true)
                    #endif
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                   #endif
                    )
#endif
{
    selectedSampleData = &sampleDatas[0];
    selectedSampleDataIndex = 0;
    recordingBufferSampleIndex = 0;
    recordModeIsOn = false;
    for (int i = 0; i < numSamplesToStore; i++)
    {
        sampleDatas[i].clear();
    }
}

BossGreatAudioProcessor::~BossGreatAudioProcessor()
{
    
}

const juce::String BossGreatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BossGreatAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BossGreatAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BossGreatAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BossGreatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BossGreatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BossGreatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BossGreatAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String BossGreatAudioProcessor::getProgramName(int index)
{
    return {};
}

void BossGreatAudioProcessor::changeProgramName(int index, const juce::String& newName)
{

}

void BossGreatAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->sampleRate = sampleRate;
}

void BossGreatAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BossGreatAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    {
        return false;
    }

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    {
        return false;
    }
   #endif
    return true;
  #endif
}
#endif

void BossGreatAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    int totalNumInputChannels = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if (!recordModeIsOn)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
        }
    }
    else
    {
        if (getPlayHead() != nullptr)
        {
            const juce::Optional<juce::AudioPlayHead::PositionInfo>& position = getPlayHead()->getPosition();
            if (position.hasValue() && position->getIsPlaying())
            {
                int sampleIndex = 0;
                while (sampleIndex < buffer.getNumSamples() && recordingBufferSampleIndex < numSamplesToRecord)
                {
                    for (int channelIndex = 0; channelIndex < selectedSampleData->getSampleBuffer().getNumChannels(); channelIndex++)
                    {
                        selectedSampleData->getSampleBuffer().setSample(channelIndex, recordingBufferSampleIndex, channelIndex < buffer.getNumChannels() ? buffer.getSample(channelIndex, sampleIndex) : 0.f);
                    }
                    sampleIndex++;
                    recordingBufferSampleIndex++;
                }
                selectedSampleData->setProcessedSampleBuffer();
                recordingBufferUpdatedEvent();
                if (recordingBufferSampleIndex >= numSamplesToRecord)
                {
                    recordingBufferSampleIndex = 0;
                    recordModeIsOn = false;
                    recordModeStateChangedEvent();
                }
            }
        }
    }
}

bool BossGreatAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BossGreatAudioProcessor::createEditor()
{
    return new BossGreatAudioProcessorEditor(*this);
}

void BossGreatAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BossGreatAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BossGreatAudioProcessor();
}

void BossGreatAudioProcessor::toggleRecordMode()
{
    recordModeIsOn = !recordModeIsOn;
    if (recordModeIsOn)
    {
        const int previousNumSamplesInRecordingBuffer = selectedSampleData->getSampleBuffer().getNumSamples();
        numSamplesToRecord = static_cast<int>(sampleRate); // Currently only 1 second of audio
        selectedSampleData->getSampleBuffer().setSize(2, numSamplesToRecord, true);
        if (previousNumSamplesInRecordingBuffer < numSamplesToRecord)
        {
            for (int channelIndex = 0; channelIndex < selectedSampleData->getSampleBuffer().getNumChannels(); channelIndex++)
            {
                for (int sampleIndex = previousNumSamplesInRecordingBuffer; sampleIndex < numSamplesToRecord; sampleIndex++)
                {
                    selectedSampleData->getSampleBuffer().setSample(channelIndex, sampleIndex, 0.f);
                }
            }
        }
        selectedSampleData->setProcessedSampleBuffer();
        recordingBufferUpdatedEvent();
    }
    else
    {
        recordingBufferSampleIndex = 0;
    }
    recordModeStateChangedEvent();
}

void BossGreatAudioProcessor::selectSampleData(int sampleDataIndex)
{
    selectedSampleDataIndex = sampleDataIndex;
    selectedSampleData = &sampleDatas[selectedSampleDataIndex];
}

int BossGreatAudioProcessor::getSelectedSampleDataIndex()
{
    return selectedSampleDataIndex;
}

bool BossGreatAudioProcessor::getRecordModeIsOn()
{
    return recordModeIsOn;
}

SampleData& BossGreatAudioProcessor::getSelectedSamplePanel()
{
    return *selectedSampleData;
}
