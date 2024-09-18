//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING

//-------------------------->IMPORTANT <----------------------------
// PLEASE ADD: JUCE_MODAL_LOOPS_PERMITTED=1 TO EXTRA PREPROCRESSOR DEFINITIONS IN THE EXPORTERS TAB INSIDE OF THE PROJUCER APP
//THIS IS REQUIRED TO RUN THE browseForMultipleFilesToOpen() FUNCTION
//THIS FUNCTION IS IN THE DOCUMENTATION OF JUCE.HOWEVER, WHEN I DONT INCLUDE THAT JUCE_MODAL LINE
//I GET AN ERROR SAYING THAT THE browseForMultipleFilesToOpen() ISNT A FUNCTION OF THE MODULE
//-------------------------->IMPORTANT <----------------------------

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "ModernLookAndFeel.h"

//==============================================================================
// Constructor for DeckGUI
// Initializes the GUI components (buttons, sliders, waveform display) and sets their properties.
// This constructor also adds the components to the visible GUI and attaches listeners for user interaction.
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse,
                 PlaylistComponent* _playlist) :
                                                 player(_player),
                                                 waveformDisplay(formatManagerToUse, cacheToUse),
                                                 playlist(_playlist)
{
    // Add buttons (play, stop, load) and cue buttons (cue1-4) to the GUI
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    
    addAndMakeVisible(cue1);
    addAndMakeVisible(cue2);
    addAndMakeVisible(cue3);
    addAndMakeVisible(cue4);

    cue1.changeWidthToFitText(); // Adjust cue1 button text width
    
    // Add volume, speed, position sliders, and the waveform display to the GUI
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    
    // Configure the zoom slider: extreme zoom ranges for detailed waveform view
    zoomSlider.setRange(0.01, 20.0, 0.01);
    zoomSlider.setValue(1.0); // Default zoom level
    zoomSlider.setTextValueSuffix("x");
    zoomSlider.setSliderStyle(Slider::LinearHorizontal);
    zoomSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    zoomSlider.addListener(this); // Add listener to handle zoomSlider changes
    addAndMakeVisible(zoomSlider); // Add the zoom slider to the GUI

    // Attach listeners for all buttons and sliders to handle user interactions
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    
    cue1.addListener(this);
    cue2.addListener(this);
    cue3.addListener(this);
    cue4.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Configure slider ranges for volume, speed, and position
    volSlider.setRange(0.0, 1.0); // Volume range between 0 (muted) and 1 (max volume)
    speedSlider.setRange(0.25, 5.0); // Playback speed from 0.25x to 5x
    volSlider.setValue(0.5); // Set default volume to 50%
    speedSlider.setValue(1.0); // Default playback speed is normal (1x)
    posSlider.setRange(0.0, 1.0); // Position slider range from start (0) to end (1) of the track
    
    // Style the position slider with a rotary style and a text box below the slider
    posSlider.setSliderStyle(Slider::Rotary);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);

    // Configure and add labels for cue, zoom, speed, volume, and playback position
    cueLabel.setText("Cue Position", dontSendNotification);
    cueLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(cueLabel);

    zoomLabel.setText("Zoom", dontSendNotification);
    zoomLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(zoomLabel);

    zoomMinusLabel.setText("(-)", dontSendNotification);
    zoomMinusLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(zoomMinusLabel);

    zoomPlusLabel.setText("(+)", dontSendNotification);
    zoomMinusLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(zoomMinusLabel);

    zoomPlusLabel.setText("(+)", dontSendNotification);
    zoomPlusLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(zoomPlusLabel);
    
    // labels for speed, volume, and position
    speedLabel.setText("Play Back Speed", dontSendNotification);
    speedLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(speedLabel);

    volLabel.setText("Volume", dontSendNotification);
    volLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(volLabel);

    posLabel.setText("Play Back Position", dontSendNotification);
    posLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(posLabel);
    
    // Customize Play and Stop button colors
    playButton.setColour(TextButton::buttonColourId, Colours::darkgrey);
    playButton.setColour(TextButton::textColourOnId, Colours::lightgreen);
    playButton.setColour(TextButton::textColourOffId, Colours::white);

    stopButton.setColour(TextButton::buttonColourId, Colours::darkgrey);
    stopButton.setColour(TextButton::textColourOnId, Colours::red);
    stopButton.setColour(TextButton::textColourOffId, Colours::white);

    // Customize Slider colors
    volSlider.setColour(Slider::thumbColourId, Colours::aqua);
    volSlider.setColour(Slider::trackColourId, Colours::darkgrey);
    
    speedSlider.setColour(Slider::thumbColourId, Colours::aqua);
    speedSlider.setColour(Slider::trackColourId, Colours::darkgrey);

    posSlider.setColour(Slider::thumbColourId, Colours::lightgreen);
    posSlider.setColour(Slider::trackColourId, Colours::darkgrey);
    
    setLookAndFeel(&modernLookAndFeel);

    // For the buttons
    playButton.setLookAndFeel(&modernLookAndFeel);
    stopButton.setLookAndFeel(&modernLookAndFeel);

    // For the sliders
    volSlider.setLookAndFeel(&modernLookAndFeel);
    speedSlider.setLookAndFeel(&modernLookAndFeel);
    posSlider.setLookAndFeel(&modernLookAndFeel);
    
    startTimer(15);
}

DeckGUI::~DeckGUI()
{
    
    setLookAndFeel(nullptr);  // Reset look and feel

    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    // Create a vertical gradient for the background (from dark grey to black)
    ColourGradient gradient(Colours::teal, 0, 0, Colours::black, 0, getHeight(), false);
    
    g.setGradientFill(gradient);
    g.fillAll ();   // Clear the background
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
}

void DeckGUI::resized()
{
    // 13 rows
    double rowH = getHeight() / 13;
    
    //deckgui takes up half the main component 
    volSlider.setBounds(0, rowH, getWidth()/1.5, rowH * 2);
    speedSlider.setBounds(0, rowH * 5, getWidth()/1.5, rowH);
    posSlider.setBounds(getWidth()/1.5, rowH/2, getWidth()/3, rowH * 6);
    waveformDisplay.setBounds(0, rowH * 7, getWidth(), rowH * 4);
    cue1.setBounds(10, rowH*12, getWidth()/4, rowH);
    cue2.setBounds(cue1.getX() + 35, rowH*12, getWidth()/4, rowH);
    cue3.setBounds(cue2.getX() + 35, rowH*12, getWidth()/4, rowH);
    cue4.setBounds(cue3.getX() + 35, rowH*12, getWidth()/4, rowH);
    playButton.setBounds(cue4.getWidth() * 2.4, rowH * 11, getWidth()/5, rowH);
    zoomSlider.setBounds(cue4.getX() + 80, cue4.getY() + 10, getWidth()/3, 20);
    stopButton.setBounds(playButton.getWidth() * 4, rowH * 11, getWidth()/5, rowH);
    loadButton.setBounds(zoomSlider.getWidth() * 2.25, cue4.getY(), getWidth()/4, rowH);

    
    // Set bounds for the labels
    cueLabel.setBounds(cue1.getX() - 30, cue1.getY() - 20, cue3.getRight() - cue1.getX(), 20); // "Cue Position" label above cue buttons
    zoomLabel.setBounds(zoomSlider.getX() + 50, zoomSlider.getY() - 20, 100, 20); // "Zoom" label above zoom slider
    zoomMinusLabel.setBounds(zoomSlider.getX() - 30, zoomSlider.getY(), 30, 20);  // (-) label to the left of the zoom slider
    zoomPlusLabel.setBounds(zoomSlider.getRight(), zoomSlider.getY(), 30, 20); 
    // Labels for sliders
    volLabel.setBounds(volSlider.getX(), volSlider.getY() - 20, volSlider.getWidth(), 20); // Above volume slider
    speedLabel.setBounds(speedSlider.getX(), speedSlider.getY() - 20, speedSlider.getWidth(), 20); // Above speed slider
    posLabel.setBounds(posSlider.getX(), posSlider.getY() - 20, posSlider.getWidth(), 20); // Above position slider
}

//==============================================================================
// Function to handle button clicks
// Determines which button was clicked and executes the corresponding action
void DeckGUI::buttonClicked(Button* button)
{
    // If the play button is clicked, start playback and update the timer
    if (button == &playButton)
    {
        player->start();           // Start playing the audio
        startTimer(50);            // Set timer to update every 50 milliseconds for smooth playback position tracking
    }
    // If the stop button is clicked, stop playback and stop the timer
    else if (button == &stopButton)
    {
        player->stop();            // Stop audio playback
        stopTimer();               // Stop the timer to halt playback position updates
    }

    // If the load button is clicked, open a file chooser to load audio files into the playlist
    if (button == &loadButton)
    {
        if (fChooser.browseForMultipleFilesToOpen())  // Open file dialog for multiple file selection
        {
            Array<File> files = fChooser.getResults(); // Get the selected files
            if (files.isEmpty())                       // Check if any files were selected
            {
                std::cout << "No files selected" << std::endl;
                return;  // Exit if no files were selected
            }

            // Add each selected file to the playlist
            for (const auto& file : files)
            {
                playlist->processFile(File{file});  // Add file to playlist and process it
            }
            playlist->tableComponent.updateContent();  // Refresh playlist display after adding files
        }
    }

    // Handling cue buttons: cue1 sets/removes the cue point and seeks to the position
    if (button == &cue1)
    {
        if (cue1HasPosition)  // Check if a cue position is already set
        {
            player->setPositionRelative(cue1Position);  // Seek to the previously set cue position
            waveformDisplay.removeCuePosition(cue1Position);  // Remove visual indicator for the cue point
            cue1HasPosition = false;  // Reset the state indicating no cue is currently set
        }
        else
        {
            cue1Position = player->getPositionRelative();  // Get the current playback position and set it as cue
            waveformDisplay.addCuePosition(cue1Position, Colours::red);  // Add visual cue marker
            cue1HasPosition = true;  // Mark cue1 as active
        }
    }

    // Similar logic for cue2, cue3, and cue4 buttons
    else if (button == &cue2)
    {
        if (cue2HasPosition)
        {
            player->setPositionRelative(cue2Position);
            waveformDisplay.removeCuePosition(cue2Position);
            cue2HasPosition = false;
        }
        else
        {
            cue2Position = player->getPositionRelative();
            waveformDisplay.addCuePosition(cue2Position, Colours::blue);
            cue2HasPosition = true;
        }
    }
    else if (button == &cue3)
    {
        if (cue3HasPosition)
        {
            player->setPositionRelative(cue3Position);
            waveformDisplay.removeCuePosition(cue3Position);
            cue3HasPosition = false;
        }
        else
        {
            cue3Position = player->getPositionRelative();
            waveformDisplay.addCuePosition(cue3Position, Colours::green);
            cue3HasPosition = true;
        }
    }
    else if (button == &cue4)
    {
        if (cue4HasPosition)
        {
            player->setPositionRelative(cue4Position);
            waveformDisplay.removeCuePosition(cue4Position);
            cue4HasPosition = false;
        }
        else
        {
            cue4Position = player->getPositionRelative();
            waveformDisplay.addCuePosition(cue4Position, Colours::yellow);
            cue4HasPosition = true;
        }
    }
}

//==============================================================================
// Function to handle slider value changes
// Adjusts parameters like volume, speed, playback position, and zoom level based on the slider's value
void DeckGUI::sliderValueChanged(Slider* slider)
{
    // Adjust the audio volume based on the value of the volSlider
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());  // Set the gain (volume) of the audio player
    }

    // Adjust the playback speed based on the value of the speedSlider
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());  // Set the speed (playback rate) of the audio player
    }

    // Adjust the playback position based on the value of the posSlider
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());  // Set the playback position relative to the total track length
    }

    // Adjust the zoom level of the waveform display based on the value of the zoomSlider
    if (slider == &zoomSlider)
    {
        waveformDisplay.setZoomLevel(zoomSlider.getValue());  // Update the zoom level in the waveform display
    }
}

//==============================================================================
// Timer callback function
// Called periodically (every X milliseconds) to update the GUI and waveform display with the current playback position
void DeckGUI::timerCallback()
{
    // Get the current playback position as a fraction of the total track length
    double relativePosition = player->getPositionRelative();
    
    // Update the waveform display to reflect the current playback position visually
    waveformDisplay.setPositionRelative(relativePosition);
}

//==============================================================================
// Function to access the WaveformDisplay object
// Allows external components to interact with the waveform display for additional functionality
WaveformDisplay& DeckGUI::getWaveFormDisplay()
{
    return waveformDisplay;  // Return a reference to the waveform display component
}
