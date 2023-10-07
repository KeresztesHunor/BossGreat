#pragma once

#include <JuceHeader.h>

class SamplePanel
{
public:
	SamplePanel(double sampleRate);
	~SamplePanel();

	juce::AudioBuffer<float>& getSample();

private:
	juce::AudioBuffer<float> sampleBuffer;
	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbnailCache;
	juce::AudioThumbnail thumbnail;
};

