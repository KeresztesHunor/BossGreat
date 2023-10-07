#include <JuceHeader.h>
#include "SamplePanel.h"

SamplePanel::SamplePanel(double sampleRate) : thumbnailCache(0), thumbnail(512, formatManager, thumbnailCache)
{
	thumbnail.setSource(new juce::AudioThumbnailBuffer(sampleBuffer, sampleBuffer.getNumSamples()));
}

SamplePanel::~SamplePanel()
{

}

juce::AudioBuffer<float>& SamplePanel::getSample()
{
	return sampleBuffer;
}
