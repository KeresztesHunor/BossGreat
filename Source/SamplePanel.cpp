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

void SamplePanel::processBuffers()
{
	removeSilence(sampleBuffer);
	removeSilence(processedSampleBuffer);
}

juce::AudioBuffer<float>& SamplePanel::getSampleBuffer()
{
	return sampleBuffer;
}

juce::AudioBuffer<float>& SamplePanel::getProcessedSampleBuffer()
{
	return processedSampleBuffer;
}

void SamplePanel::removeSilence(juce::AudioBuffer<float>& buffer)
{
	if (buffer.getNumChannels() > 0)
	{
		int sampleIndex = buffer.getNumSamples() - 1;
		while (sampleIndex >= 0 && silenceOnAllChannels(buffer, sampleIndex))
		{
			sampleIndex--;
		}
		buffer.setSize(buffer.getNumChannels(), sampleIndex + 1, true);
	}
}

bool SamplePanel::silenceOnAllChannels(juce::AudioBuffer<float>& buffer, int sampleIndex)
{
	int channelIndex = 0;
	while (channelIndex < buffer.getNumChannels() && buffer.getSample(channelIndex, sampleIndex) == 0)
	{
		channelIndex++;
	}
	return channelIndex >= buffer.getNumChannels();
}
