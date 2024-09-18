//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/*
    The DJAudioPlayer class is responsible for managing audio playback functionality
    in the DJ application. This class allows loading of audio files, controlling
    playback settings such as volume (gain), speed, and position, and interfacing
    with the JUCE audio engine to deliver sound.
*/
class DJAudioPlayer : public AudioSource {
public:
    // Constructor and Destructor
    /**
     * Constructor: Initializes the DJAudioPlayer with the provided AudioFormatManager.
     * The AudioFormatManager handles decoding different audio file formats.
     * @param _formatManager A reference to the AudioFormatManager that handles file decoding.
     */
    DJAudioPlayer(AudioFormatManager& _formatManager);

    /**
     * Destructor: Cleans up any resources used by DJAudioPlayer.
     */
    ~DJAudioPlayer();

    // AudioSource methods (overrides)
    /**
     * Prepares the DJAudioPlayer for playback by configuring the sample rate
     * and block size expected during playback.
     * @param samplesPerBlockExpected The number of audio samples per block.
     * @param sampleRate The sample rate (samples per second) for audio playback.
     */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Processes the next block of audio data and fills the provided buffer.
     * This method is repeatedly called by the audio system during playback.
     * @param bufferToFill A reference to the buffer to be filled with audio data.
     */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    /**
     * Releases any resources or memory used by the player.
     * This is called when audio playback is no longer required.
     */
    void releaseResources() override;

    // Playback control methods
    /**
     * Loads an audio file from a URL into the player for playback.
     * @param audioURL The URL of the audio file to load.
     */
    void loadURL(URL audioURL);

    /**
     * Sets the gain (volume) level of the player.
     * @param gain The desired gain level (0.0 = silent, 1.0 = full volume).
     */
    void setGain(double gain);

    /**
     * Sets the playback speed by adjusting the resampling ratio.
     * @param ratio The speed ratio (1.0 = normal speed, >1.0 = faster, <1.0 = slower).
     */
    void setSpeed(double ratio);

    /**
     * Sets the playback position in seconds.
     * @param posInSecs The position to set in seconds.
     */
    void setPosition(double posInSecs);

    /**
     * Sets the playback position as a fraction of the total track length.
     * @param pos The relative position (0.0 = start, 1.0 = end of the track).
     */
    void setPositionRelative(double pos);

    /**
     * Starts audio playback from the current position.
     */
    void start();

    /**
     * Stops audio playback.
     */
    void stop();

    // Utility methods
    /**
     * Returns the current position of the playhead as a fraction of the total track length.
     * @return A value between 0.0 and 1.0 representing the relative position.
     */
    double getPositionRelative();

    /**
     * Returns the total length of the currently loaded track in seconds.
     * @return The length of the track in seconds.
     */
    double getLength();

private:
    // Audio Components
    AudioFormatManager& formatManager;  // Reference to the audio format manager
    std::unique_ptr<AudioFormatReaderSource> readerSource;  // Manages audio reading from the file
    AudioTransportSource transportSource;  // Handles the transport control (play, stop, etc.)
    ResamplingAudioSource resampleSource{&transportSource, false, 2};  // Adjusts playback speed

};
