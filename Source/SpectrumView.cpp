#include <JuceHeader.h>
#include "SpectrumView.h"

SpectrumView::SpectrumView() : BorderedComponent(1, juce::Colours::white)
{
    
}

SpectrumView::~SpectrumView()
{

}

void SpectrumView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    BorderedComponent::paint(g);
}

void SpectrumView::resized()
{
    
}
