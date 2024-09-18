//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#include "ModernLookAndFeel.h"

//==============================================================================
// Constructor for ModernLookAndFeel
// Sets the default colors for sliders and buttons. These colors will be applied to
// any components that use this LookAndFeel object.
ModernLookAndFeel::ModernLookAndFeel()
{
    // Set color for slider thumb
    setColour(Slider::thumbColourId, Colours::aqua);

    // Set color for slider track
    setColour(Slider::trackColourId, Colours::darkgrey);

    // Set color for button background
    setColour(TextButton::buttonColourId, Colours::darkgrey);

    // Set color for button text (when the button is not pressed)
    setColour(TextButton::textColourOffId, Colours::white);

    // Set color for button text (when the button is pressed)
    setColour(TextButton::textColourOnId, Colours::lightgreen);
}

//==============================================================================
// Method to draw the background of a button
// This method customizes the appearance of buttons based on mouse interaction.
// It draws a rounded rectangle and adjusts the color depending on whether the
// button is being hovered over or pressed.
void ModernLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                                             bool isMouseOverButton, bool isButtonDown)
{
    // Get the area of the button and reduce its size slightly for a padding effect
    auto buttonArea = button.getLocalBounds().toFloat().reduced(2.0f);

    // Start with the base background color
    Colour baseColour = backgroundColour;

    // If the mouse is hovering over the button, make the color slightly brighter
    if (isMouseOverButton)
        baseColour = baseColour.brighter(0.2f);

    // If the button is pressed down, make the color slightly darker
    if (isButtonDown)
        baseColour = baseColour.darker(0.2f);

    // Set the fill color to the modified base color
    g.setColour(baseColour);

    // Fill the button area with a rounded rectangle
    g.fillRoundedRectangle(buttonArea, 6.0f);

    // Add a slight shadow effect around the button
    g.setColour(Colours::black.withAlpha(0.2f));
    g.drawRoundedRectangle(buttonArea, 6.0f, 1.0f);
}

//==============================================================================
// Method to draw a linear slider
// This method customizes the appearance of sliders, including the track and thumb (handle).
// The track is drawn as a rectangle, and the thumb is drawn as a circle.
void ModernLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         const Slider::SliderStyle, Slider& slider)
{
    // Set the color for the slider track
    g.setColour(Colours::aqua);

    // Define the area for the slider track
    Rectangle<float> track(x, y, width, height);

    // Draw the slider track as a filled rectangle
    g.fillRect(Rectangle<float>(sliderPos, height / 2.0f - 2, width, 4));

    // Set the color for the slider thumb (handle)
    g.setColour(Colours::lightgreen);

    // Draw the slider thumb as a filled ellipse (circle)
    g.fillEllipse(Rectangle<float>(sliderPos - 10, height / 2.0f - 10, 20, 20));
}
