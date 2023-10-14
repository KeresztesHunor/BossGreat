#include "MainColours.h"
#include "WaveFormDisplay.h"

WaveFormDisplay::WaveFormDisplay(BossGreatAudioProcessor& p) : BorderedComponent(1, juce::Colours::white), audioProcessor(p)
{

}

WaveFormDisplay::~WaveFormDisplay()
{

}

void WaveFormDisplay::paint(juce::Graphics& g)
{
    g.fillAll(MainColours::colour2);
    BorderedComponent::paint(g);
    g.setColour(juce::Colours::white);
    juce::AudioBuffer<float>& buffer = audioProcessor.getSelectedSamplePanel().getProcessedSampleBuffer();
    if (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0)
    {
        float sampleChunkSize = buffer.getNumSamples() / static_cast<float>(getWidth());
        int roundedChunkBorderLoewerIndex = 0;
        for (int pixelIndex = 0; pixelIndex < getWidthWithoutBorder(); pixelIndex++)
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
            float minY = juce::jmap<float>(minValue, -1.f, 1.f, getHeightWithoutBorder(), 0.f);
            float maxY = juce::jmap<float>(maxValue, -1.f, 1.f, getHeightWithoutBorder(), 0.f);
            g.drawLine(pixelIndex, maxY, pixelIndex, minY + 1);
        }
    }
    else
    {
        float halfHeinght = getHeightWithoutBorder() / 2.f;
        g.drawLine(0, halfHeinght, getWidthWithoutBorder(), halfHeinght);
    }
}

void WaveFormDisplay::resized()
{

}
