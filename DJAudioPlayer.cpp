//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#include "DJAudioPlayer.h"

//==============================================================================
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
    // Constructor - initializes DJAudioPlayer with the given AudioFormatManager
}

DJAudioPlayer::~DJAudioPlayer()
{
    // Destructor - cleans up any resources used by DJAudioPlayer
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare the audio sources (transportSource and resampleSource) for playback
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    // Fetch the next block of audio data through the resample source
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    // Release any resources used by the transportSource and resampleSource
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    // Load an audio file from the provided URL and set it up for playback

    // Create an audio reader for the given URL
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr)  // If the file is valid
    {
        // Create a new AudioFormatReaderSource for playback
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));

        // Set the transport source to use the new source
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

        // Store the new source in readerSource for memory management
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    // Set the gain (volume) of the transportSource
    // Gain must be between 0.0 (silent) and 1.0 (full volume)

    if (gain < 0.0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);  // Set the gain if it's within range
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    // Set the playback speed by adjusting the resampling ratio
    // Ratio must be between 0.25 (slower) and 5.0 (faster)

    if (ratio < 0.25 || ratio > 5.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0.25 and 5.0" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);  // Set the resampling ratio for speed adjustment
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    // Set the playback position in absolute time (seconds)
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    // Set the playback position as a fraction of the total track length
    // Position must be between 0.0 (start) and 1.0 (end)

    if (pos < 0.0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;  // Calculate the absolute position in seconds
        setPosition(posInSecs);  // Set the position in seconds
    }
}

void DJAudioPlayer::start()
{
    // Start playback
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    // Stop playback
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    // Return the current playback position as a fraction of the total track length
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLength()
{
    // Return the total length of the loaded track in seconds
    return transportSource.getLengthInSeconds();
}
