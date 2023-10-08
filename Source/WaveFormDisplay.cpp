#include "WaveFormDisplay.h"

WaveFormDisplay::WaveFormDisplay(BossGreatAudioProcessor& p) : juce::Component("Waveform display"), audioProcessor(p)
{

}

WaveFormDisplay::~WaveFormDisplay()
{

}

void WaveFormDisplay::paint (juce::Graphics& g)
{
    juce::AudioBuffer<float>& buffer = audioProcessor.getSelectedSamplePanel().getProcessedSampleBuffer();
    if (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0)
    {
        g.setColour(juce::Colours::white);
        float sampleChunkSize = buffer.getNumSamples() / static_cast<float>(getWidth());
        int roundedChunkBorderLoewerIndex = 0;
        for (int pixelIndex = 0; pixelIndex < getWidth(); pixelIndex++)
        {
            int roundedChunkBorderUpperIndex = static_cast<int>(std::round(sampleChunkSize * (pixelIndex + 1)));
            float minValue = buffer.getSample(0, roundedChunkBorderLoewerIndex);
            float maxValue = minValue;
            for (int sampleIndex = roundedChunkBorderLoewerIndex + 1; sampleIndex < roundedChunkBorderUpperIndex; sampleIndex++)
            {
                float sampleValue = buffer.getSample(0, sampleIndex);
                if (sampleValue < minValue)
                {
                    minValue = sampleValue;
                }
                if (sampleValue > maxValue)
                {
                    maxValue = sampleValue;
                }
            }
            roundedChunkBorderLoewerIndex = roundedChunkBorderUpperIndex;
            float minY = juce::jmap<float>(minValue, -1.f, 1.f, getHeight(), 0.f);
            float maxY = juce::jmap<float>(maxValue, -1.f, 1.f, getHeight(), 0.f);
            g.drawLine(pixelIndex, maxY, pixelIndex, minY);
        }
    }
}

void WaveFormDisplay::resized()
{

}
