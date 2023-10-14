#include <JuceHeader.h>
#include "MainColours.h"
#include "ControlsPanel.h"

ControlsPanel::ControlsPanel() : BorderedComponent(1, juce::Colours::white)
{
    
}

ControlsPanel::~ControlsPanel()
{

}

void ControlsPanel::paint (juce::Graphics& g)
{
    g.fillAll(MainColours::colour1);
    BorderedComponent::paint(g);
}

void ControlsPanel::resized()
{
    
}
