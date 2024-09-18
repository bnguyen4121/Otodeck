//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


//-------------------------->IMPORTANT <----------------------------
// PLEASE ADD: JUCE_MODAL_LOOPS_PERMITTED=1 TO EXTRA PREPROCRESSOR DEFINITIONS IN THE EXPORTERS TAB INSIDE OF THE PROJUCER APP
//THIS IS REQUIRED TO RUN THE browseForMultipleFilesToOpen() FUNCTION
//THIS FUNCTION IS IN THE DOCUMENTATION OF JUCE HOWEVER WHEN I DONT INCLUDE THAT JUCE_MODAL LINE
//I GET AN ERROR SAYING THAT THE browseForMultipleFilesToOpen() ISNT A FUNCTION OF THE MODULE 
//-------------------------->IMPORTANT <----------------------------

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"
//==============================================================================
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    //formatManager in maincomponent so left deck and right can talk to each other
    AudioFormatManager formatManager;
    //    cache 100 tracks at one time
    AudioThumbnailCache thumbCache{ 100 };
    
    DJAudioPlayer player1{ formatManager };
    //  pass formatManager and thumbCache so deckGUI is ready to receieve the data from mainComponent
    
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, &playlist };

    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &playlist };
    PlaylistComponent playlist{ &player1, &player2, formatManager,thumbCache,&deckGUI1.getWaveFormDisplay(),&deckGUI2.getWaveFormDisplay()};

    MixerAudioSource mixerSource;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
