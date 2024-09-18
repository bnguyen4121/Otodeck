//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING



#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
// Constructor for WaveformDisplay
// Initializes the audio thumbnail and FFT setup, and adds the change listener for waveform updates.
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse),
      fileLoaded(false),
      position(0),
      fftSize(2048),
      forwardFFT(11), // FFT size is 2^11
      audioDataBuffer(2048),
      fftDataBuffer(4096),
      windowFunction(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    audioThumb.addChangeListener(this); // Attach change listener to the audio thumbnail
}

WaveformDisplay::~WaveformDisplay()
{
    // Destructor
}

void WaveformDisplay::paint(Graphics& g)
{
    // Clear the background using the current LookAndFeel color
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // Draw a grey border around the component
    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    // If a file is loaded, draw the waveform, playhead, and cues
    if (fileLoaded)
    {
        drawWaveform(g);
        drawPlayhead(g);
        drawCues(g);
    }
    // If no file is loaded, display placeholder text
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(), Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
    // Resizing logic can be handled here if necessary
}

//==============================================================================
// Method to load a URL into the waveform display
// This clears any previous file and sets the new file for display.
void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear(); // Clear any previously loaded audio
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL)); // Load the new audio file

    // If the file is successfully loaded, trigger a repaint to update the display
    if (fileLoaded)
    {
        std::cout << "WaveformDisplay: File loaded successfully!" << std::endl;
        repaint();
    }
    else
    {
        std::cout << "WaveformDisplay: Failed to load file." << std::endl;
    }
}

//==============================================================================
// Responds to any changes in the audio thumbnail (e.g., file loaded, waveform updated)
// Triggers a repaint to update the visual display of the waveform.
void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}

//==============================================================================
// Sets the playback position of the waveform display
// This is typically called to update the playhead as the track progresses.
void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint(); // Repaint to move the playhead to the new position
    }
}

//==============================================================================
// Adds a cue point to the waveform at the specified position and with the specified color
// The cue is visually represented on the waveform.
void WaveformDisplay::addCuePosition(double pos, Colour cueColor)
{
    if (pos >= 0.0 && pos <= 1.0)  // Ensure the cue position is within valid range
    {
        cuePositions.push_back(std::make_pair(pos, cueColor));  // Add cue point to the list
        repaint();  // Trigger a repaint to display the cue
    }
}

//==============================================================================
// Removes a cue point from the waveform at the specified position
// The cue is visually removed from the waveform.
void WaveformDisplay::removeCuePosition(double pos)
{
    // Find and erase the cue point that matches the position
    cuePositions.erase(std::remove_if(cuePositions.begin(), cuePositions.end(),
                                      [pos](const std::pair<double, Colour>& cue) { return cue.first == pos; }),
                       cuePositions.end());
    repaint();  // Trigger a repaint to update the display without the removed cue
}

//==============================================================================
// Performs frequency analysis using FFT on the audio data
// Analyzes the visible range of the audio file for frequency content visualization.
void WaveformDisplay::performFrequencyAnalysis()
{
    if (!fileLoaded || audioThumb.getTotalLength() <= 0)
        return;

    // Get the visible range of the audio file for analysis
    double startTime = audioThumb.getTotalLength() * position - (audioThumb.getTotalLength() * 0.5);
    if (startTime < 0) startTime = 0;
    double endTime = startTime + fftSize / 44100.0;

    // Buffer to store min and max values from the audio thumbnail
    float minValue, maxValue;

    // Populate the FFT buffer with audio data for analysis
    for (int i = 0; i < fftSize; ++i)
    {
        audioThumb.getApproximateMinMax(startTime + (i / (double)fftSize) * (endTime - startTime),
                                        startTime + ((i + 1) / (double)fftSize) * (endTime - startTime),
                                        0, minValue, maxValue);
        fftDataBuffer[i] = (minValue + maxValue) / 2.0f;
    }

    // Apply windowing to reduce artifacts
    windowFunction.multiplyWithWindowingTable(fftDataBuffer.data(), fftSize);

    // Perform the FFT analysis
    forwardFFT.performFrequencyOnlyForwardTransform(fftDataBuffer.data());
}

//==============================================================================
// Returns a color based on the frequency for visualizing frequency content
// The color is mapped based on the frequency range of human hearing (20Hz - 20kHz).
juce::Colour WaveformDisplay::getColorForFrequency(float frequency)
{
    // Define the frequency range for human hearing
    float minFrequency = 20.0f;
    float maxFrequency = 20000.0f;

    // Clamp the frequency value to stay within this range
    frequency = juce::jlimit(minFrequency, maxFrequency, frequency);

    // Map the frequency to a hue value (0.0 to 1.0)
    float hue = juce::jmap(frequency, minFrequency, maxFrequency, 0.0f, 1.0f);

    // Return the color based on the hue, with full saturation and brightness
    return juce::Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f);
}

//==============================================================================
// Draws the waveform of the loaded audio file
// The waveform is visualized according to the current zoom level and play position.
void WaveformDisplay::drawWaveform(Graphics& g)
{
    // Get the total length of the loaded audio file
    double totalLength = audioThumb.getTotalLength();

    // Calculate the visible range of the waveform based on zoom level and playhead position
    double visibleDuration = totalLength / zoomLevel;
    double visibleRangeStart = totalLength * position - (visibleDuration * 0.5);
    double visibleRangeEnd = totalLength * position + (visibleDuration * 0.5);

    // Ensure the visible range is within bounds
    if (visibleRangeStart < 0) visibleRangeStart = 0;
    if (visibleRangeEnd > totalLength) visibleRangeEnd = totalLength;

    // Draw the waveform with frequency-based colors
    for (int i = 0; i < getWidth(); ++i)
    {
        float samplePosition = visibleRangeStart + (i / float(getWidth())) * (visibleRangeEnd - visibleRangeStart);
        float minValue, maxValue;
        audioThumb.getApproximateMinMax(samplePosition, samplePosition + 0.001, 0, minValue, maxValue);

        float waveformYMin = jmap(minValue, -1.0f, 1.0f, float(getHeight()), 0.0f);
        float waveformYMax = jmap(maxValue, -1.0f, 1.0f, float(getHeight()), 0.0f);

        float frequency = (i * (44100 / 2)) / getWidth();  // Scale to the sample rate
        Colour color = getColorForFrequency(frequency);

        g.setColour(color);
        g.drawLine(i, waveformYMin, i, waveformYMax);  // Draw the waveform line
    }
}

//==============================================================================
// Draws the playhead indicator on the waveform
// The playhead moves according to the current playback position.
void WaveformDisplay::drawPlayhead(Graphics& g)
{
    g.setColour(Colours::lightgreen);
    int playheadX;

    double endThreshold = 0.95;
    if (position <= endThreshold)
    {
        playheadX = getWidth() / 2;  // Playhead at the center if within threshold
    }
    else
    {
        double remainingTrackPosition = (position - endThreshold) / (1.0 - endThreshold);
        playheadX = getWidth() / 2 + remainingTrackPosition * (getWidth() / 2);  // Shift to the right as track nears end
    }

    g.drawLine(playheadX, 0, playheadX, getHeight(), 2.0f);  // Draw the playhead line
}

//==============================================================================
// Draws the cue markers on the waveform
// Cue points are displayed at the correct positions relative to the waveform.
void WaveformDisplay::drawCues(Graphics& g)
{
    for (const auto& cue : cuePositions)
    {
        g.setColour(cue.second);  // Set the color for the cue marker
        double cueXPosition = getWidth() * (cue.first - position + 0.5);  // Calculate the position for the cue marker

        // Only draw the cue if it's visible within the current waveform view
        if (cueXPosition >= 0 && cueXPosition <= getWidth())
        {
            g.drawLine(cueXPosition, 0, cueXPosition, getHeight(), 2.0f);  // Draw the cue marker
        }
    }
}

//==============================================================================
// Adjusts the zoom level of the waveform display
// The zoom level is clamped between 1.0 (normal) and 20.0 (max zoom).
void WaveformDisplay::setZoomLevel(double newZoomLevel)
{
    zoomLevel = juce::jlimit(1.0, 20.0, newZoomLevel);  // Clamp the zoom level to a reasonable range
    repaint();  // Trigger a repaint to apply the new zoom level
}

//==============================================================================
// Retrieves the sample at a given index in the waveform
// This is useful for analyzing specific points in the waveform.
float WaveformDisplay::getSampleAtIndex(int sampleIndex)
{
    int numChannels = 1; // Assume mono audio
    float sample = 0.0f;

    // Get approximate min and max values for the sample at the given index
    if (audioThumb.getNumChannels() > 0)
    {
        AudioBuffer<float> buffer(numChannels, 1);
        audioThumb.getApproximateMinMax(sampleIndex / 44100.0, (sampleIndex + 1) / 44100.0, 0, sample, sample);
    }

    return sample;
}
