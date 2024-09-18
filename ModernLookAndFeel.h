//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#pragma once

#include <JuceHeader.h>

/*
    The ModernLookAndFeel class is a custom LookAndFeel class derived from LookAndFeel_V4.
    It defines the appearance of buttons and sliders in the application by overriding
    key drawing methods. This custom look is used across the DJ application to provide
    a modern, visually consistent interface.
*/
class ModernLookAndFeel : public LookAndFeel_V4
{
public:
    // Constructor
    /**
     * Constructor: Initializes the ModernLookAndFeel object and sets default colors
     * for sliders and buttons, which will be applied to all components using this LookAndFeel.
     */
    ModernLookAndFeel();

    // Button background customization
    /**
     * Draws the background of buttons, customizing their appearance based on their state
     * (normal, hovered, or pressed). This method ensures a rounded rectangular appearance
     * with a slight shadow effect.
     *
     * @param g The graphics context to draw with.
     * @param button The button to draw.
     * @param backgroundColour The base color of the button's background.
     * @param isMouseOverButton Whether the mouse is hovering over the button.
     * @param isButtonDown Whether the button is currently pressed.
     */
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                              bool isMouseOverButton, bool isButtonDown) override;

    // Slider customization
    /**
     * Customizes the appearance of linear sliders, drawing both the slider track and thumb
     * (handle). The track is drawn as a rectangle, and the thumb as a circle.
     *
     * @param g The graphics context to draw with.
     * @param x The x-coordinate of the slider.
     * @param y The y-coordinate of the slider.
     * @param width The width of the slider.
     * @param height The height of the slider.
     * @param sliderPos The position of the slider thumb along the track.
     * @param minSliderPos The minimum position of the slider.
     * @param maxSliderPos The maximum position of the slider.
     * @param sliderStyle The style of the slider (linear, rotary, etc.).
     * @param slider The slider object being drawn.
     */
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const Slider::SliderStyle sliderStyle, Slider& slider) override;
};
