#include "SampleData.h"

SampleData::SampleData()
{

}

SampleData::~SampleData()
{

}

void SampleData::clear()
{
	sampleBuffer.clear();
	processedSampleBuffer.clear();
}

void SampleData::setProcessedSampleBuffer()
{
	processedSampleBuffer = sampleBuffer;
}

void SampleData::processBuffers()
{
	removeSilence(sampleBuffer);
	removeSilence(processedSampleBuffer);
}

juce::AudioBuffer<float>& SampleData::getSampleBuffer()
{
	return sampleBuffer;
}

juce::AudioBuffer<float>& SampleData::getProcessedSampleBuffer()
{
	return processedSampleBuffer;
}

void SampleData::removeSilence(juce::AudioBuffer<float>& buffer)
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

bool SampleData::silenceOnAllChannels(juce::AudioBuffer<float>& buffer, int sampleIndex)
{
	int channelIndex = 0;
	while (channelIndex < buffer.getNumChannels() && buffer.getSample(channelIndex, sampleIndex) == 0)
	{
		channelIndex++;
	}
	return channelIndex >= buffer.getNumChannels();
}
