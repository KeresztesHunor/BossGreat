#include "SamplePanel.h"

SamplePanel::SamplePanel()
{

}

SamplePanel::~SamplePanel()
{

}

void SamplePanel::clear()
{
	sampleBuffer.clear();
	processedSampleBuffer.clear();
}

void SamplePanel::setProcessedSampleBuffer()
{
	processedSampleBuffer = sampleBuffer;
}

juce::AudioBuffer<float>& SamplePanel::getSampleBuffer()
{
	return sampleBuffer;
}

juce::AudioBuffer<float>& SamplePanel::getProcessedSampleBuffer()
{
	return processedSampleBuffer;
}
