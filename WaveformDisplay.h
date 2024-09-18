//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

/*
    The WaveformDisplay class is responsible for visualizing the waveform of an
    audio file. It allows for displaying playback position, adding/removing cue points,
    adjusting zoom level, and performing frequency analysis using FFT (Fast Fourier Transform).
*/
class WaveformDisplay : public Component,
                        public ChangeListener
{
public:
    // Constructor and Destructor
    /**
     * Constructor: Initializes the WaveformDisplay with the provided format manager
     * and thumbnail cache.
     * @param formatManagerToUse A reference to the AudioFormatManager used for decoding audio.
     * @param cacheToUse A reference to the AudioThumbnailCache used for caching the audio thumbnails.
     */
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);

    /**
     * Destructor: Cleans up any resources used by WaveformDisplay.
     */
    ~WaveformDisplay() override;

    // Component methods (overrides)
    /**
     * Paints the waveform and any other visual elements like cue points and the playhead.
     * @param g The graphics context used for drawing.
     */
    void paint(Graphics&) override;

    /**
     * Resizes the component and adjusts layout as necessary.
     */
    void resized() override;

    // Waveform display and interaction methods
    /**
     * Loads an audio file into the waveform display from the provided URL.
     * @param audioURL The URL of the audio file to load.
     */
    void loadURL(URL audioURL);

    /**
     * Responds to changes in the audio thumbnail and updates the waveform visualization.
     * @param source The ChangeBroadcaster that triggered this callback.
     */
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /**
     * Sets the relative position of the playhead in the waveform.
     * @param pos The relative position (0.0 = start, 1.0 = end of the track).
     */
    void setPositionRelative(double pos);

    /**
     * Adds a cue point at the specified position and colors it.
     * @param pos The position to add the cue point (relative to the track length).
     * @param cueColor The color to display the cue point.
     */
    void addCuePosition(double pos, Colour cueColor);

    /**
     * Removes a cue point at the specified position.
     * @param pos The position of the cue point to remove.
     */
    void removeCuePosition(double pos);

    /**
     * Sets the zoom level of the waveform display.
     * @param newZoomLevel The desired zoom level (1.0 = normal, >1.0 = zoomed in, <1.0 = zoomed out).
     */
    void setZoomLevel(double newZoomLevel);

private:
    // Audio and waveform components
    /** AudioThumbnail is used to load and visualize the waveform of the audio file. */
    AudioThumbnail audioThumb;

    /** Indicates whether an audio file is currently loaded. */
    bool fileLoaded;

    /** Current playback position (relative to the total length of the audio file). */
    double position;

    // Zoom properties
    /** Zoom factor for displaying the waveform (1.0 = no zoom). */
    double zoomFactor = 1.0;

    /** Zoom level controlled externally (set via DeckGUI). */
    double zoomLevel;

    // FFT and frequency analysis
    /** Size of the FFT window for frequency analysis. */
    int fftSize;

    /** Fast Fourier Transform (FFT) object for performing frequency analysis. */
    dsp::FFT forwardFFT;

    /** Buffer for storing raw audio data for analysis and visualization. */
    std::vector<float> audioDataBuffer;

    /** Buffer for storing FFT data for frequency analysis. */
    std::vector<float> fftDataBuffer;

    /** Windowing function for smoothing the FFT data. */
    dsp::WindowingFunction<float> windowFunction;

    // Cue points
    /** A vector of pairs that store cue positions and their corresponding colors. */
    std::vector<std::pair<double, Colour>> cuePositions;

    // Private methods
    /**
     * Performs the FFT (frequency analysis) on the loaded audio data.
     */
    void performFrequencyAnalysis();

    /**
     * Returns a color corresponding to a specific frequency.
     * @param frequency The frequency for which to get the color.
     * @return A color representing the specified frequency.
     */
    juce::Colour getColorForFrequency(float frequency);

    /**
     * Returns the amplitude value of a specific sample at the provided index.
     * @param sampleIndex The index of the sample to retrieve.
     * @return The amplitude value at the given sample index.
     */
    float getSampleAtIndex(int sampleIndex);

    /**
     * Draws the waveform based on the loaded audio file.
     * @param g The graphics context used for drawing.
     */
    void drawWaveform(Graphics& g);

    /**
     * Draws the playhead to show the current playback position.
     * @param g The graphics context used for drawing.
     */
    void drawPlayhead(Graphics& g);

    /**
     * Draws the cue points on the waveform at their respective positions.
     * @param g The graphics context used for drawing.
     */
    void drawCues(Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
