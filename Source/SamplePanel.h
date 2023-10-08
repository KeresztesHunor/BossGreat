#pragma once

#include <JuceHeader.h>

class SamplePanel
{
public:
	SamplePanel();
	~SamplePanel();

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
};
