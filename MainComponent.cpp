//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1200, 800);
    
    // Load the playlist on app startup
    File saveFile = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("playlist.json");
    if (saveFile.existsAsFile())
    {
        playlist.loadPlaylistFromFile(saveFile);  // Load the saved playlist
    }
    else
    {
        std::cout << "No playlist found, starting with an empty playlist." << std::endl;
    }

    addAndMakeVisible(playlist);  // Ensure playlist component is visible

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlist);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // Save the playlist when the application closes
    File saveFile = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("playlist.json");
    playlist.savePlaylistToFile(saveFile);  // Call the save method from the PlaylistComponent
    
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight()/2);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/2);
    playlist.setBounds(0, getHeight()/2, getWidth(), getHeight());
}
