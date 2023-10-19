#pragma once

#include <JuceHeader.h>

class SampleData
{
public:
	SampleData();
	~SampleData();

	void clear();
	void setProcessedSampleBuffer();
	void processBuffers();

	juce::AudioBuffer<float>& getSampleBuffer();
	juce::AudioBuffer<float>& getProcessedSampleBuffer();

private:
	juce::AudioBuffer<float> sampleBuffer;
	juce::AudioBuffer<float> processedSampleBuffer;

	void removeSilence(juce::AudioBuffer<float>&);

	bool silenceOnAllChannels(juce::AudioBuffer<float>&, int);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleData)
};
