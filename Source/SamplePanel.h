#pragma once

#include <JuceHeader.h>

class SamplePanel
{
public:
	SamplePanel();
	~SamplePanel();

	void clear();
	void setProcessedSampleBuffer();

	juce::AudioBuffer<float>& getSampleBuffer();
	juce::AudioBuffer<float>& getProcessedSampleBuffer();

private:
	juce::AudioBuffer<float> sampleBuffer;
	juce::AudioBuffer<float> processedSampleBuffer;
};
