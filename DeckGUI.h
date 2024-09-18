//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "ModernLookAndFeel.h"
//==============================================================================
/*
    The DeckGUI class handles the individual deck controls for the DJ app.
    This class allows interaction with audio players, sliders, and waveform displays.
*/
class DeckGUI    : public juce::Component,
                   public juce::Button::Listener,
                   public juce::Slider::Listener,
                   public juce::Timer
{
public:
    // Constructor and Destructor
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse, PlaylistComponent* playlist);
    ~DeckGUI() override;

    // Component methods
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //getter method to return the waveform
    WaveformDisplay& getWaveFormDisplay();
private:
    // GUI Components
    juce::TextButton playButton { "Play"};
    juce::TextButton stopButton { "Stop" };
    juce::TextButton loadButton { "Load Tracks" };
    juce::Slider volSlider;   // Volume control slider
    juce::Slider speedSlider; // Speed control slider
    juce::Slider posSlider;   // Position control slider
    Slider zoomSlider; // Zoom slider to control the waveform

//  cue buttons
    ToggleButton cue1{"1"};
    ToggleButton cue2{"2"};
    ToggleButton cue3{"3"};
    ToggleButton cue4{"4"};
    
    // Labels for cue and zoom
    Label cueLabel;
    Label zoomLabel;
    Label zoomMinusLabel;
    Label zoomPlusLabel;
    
    // labels for volume, speed, and position sliders
    Label volLabel;
    Label speedLabel;
    Label posLabel;

    
    //initialization variables for the 4 cue position buttons
    bool cue1HasPosition = false;
    double cue1Position = -1.0;

    bool cue2HasPosition = false;
    double cue2Position = -1.0;

    bool cue3HasPosition = false;
    double cue3Position = -1.0;

    bool cue4HasPosition = false;
    double cue4Position = -1.0;
    
    
    // Utility objects
    FileChooser fChooser {"Select a file..."};  // File chooser for loading audio files
    
    // Button::Listener method
    void buttonClicked (juce::Button* button) override;

    // Slider::Listener method
    void sliderValueChanged (juce::Slider* slider) override;

    // Timer callback
    void timerCallback() override;
    
    // Audio Components
    DJAudioPlayer* player;  // The audio player controlled by this deck
    PlaylistComponent* playlist;
    WaveformDisplay waveformDisplay;  // Display for the waveform of the audio file
    
    //initialization of LookAndFeel method
    ModernLookAndFeel modernLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
