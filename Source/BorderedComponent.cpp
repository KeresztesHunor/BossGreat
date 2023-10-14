#include <JuceHeader.h>
#include "BorderedComponent.h"

BorderedComponent::BorderedComponent(int borderThickness, juce::Colour borderColour) : juce::Component()
{
    this->borderThickness = borderThickness;
    this->borderColour = borderColour;
}

BorderedComponent::~BorderedComponent()
{

}

void BorderedComponent::paint (juce::Graphics& g)
{
    g.setColour(borderColour);
    g.drawRect(0, 0, getWidth(), getHeight(), borderThickness);
}

void BorderedComponent::resized()
{
    
}

int BorderedComponent::getWidthWithoutBorder()
{
    return getWidth() - borderThickness * 2;
}

int BorderedComponent::getHeightWithoutBorder()
{
    return getHeight() - borderThickness * 2;
}

int BorderedComponent::getBorderThickness()
{
    return borderThickness;
}
