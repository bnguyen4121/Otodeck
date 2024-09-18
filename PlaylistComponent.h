//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "ModernLookAndFeel.h"

//==============================================================================
/*
    The PlaylistComponent class represents a GUI component that displays a playlist of audio tracks.
    It allows users to load, search, and interact with audio tracks, as well as control playback
    using the DJAudioPlayer objects. The playlist can be saved and loaded from a file, and the
    interface supports drag-and-drop functionality for loading new tracks.
*/
class PlaylistComponent : public Component,
                          public TableListBoxModel,
                          public Button::Listener,
                          public FileDragAndDropTarget,
                          public TextEditor::Listener
{
public:
    // Constructor and Destructor
    /**
     * Constructor: Initializes the PlaylistComponent with two DJAudioPlayer objects,
     * an AudioFormatManager, an AudioThumbnailCache, and two WaveformDisplay components.
     *
     * @param player1 Pointer to the first DJAudioPlayer.
     * @param player2 Pointer to the second DJAudioPlayer.
     * @param formatManagerToUse Reference to the AudioFormatManager used for file decoding.
     * @param cacheToUse Reference to the AudioThumbnailCache used for caching waveforms.
     * @param waveformDisplay Pointer to the first WaveformDisplay.
     * @param waveformDisplay2 Pointer to the second WaveformDisplay.
     */
    PlaylistComponent(DJAudioPlayer* player1,
                      DJAudioPlayer* player2,
                      AudioFormatManager& formatManagerToUse,
                      AudioThumbnailCache& cacheToUse,
                      WaveformDisplay* waveformDisplay,
                      WaveformDisplay* waveformDisplay2);

    /**
     * Destructor: Cleans up resources used by the PlaylistComponent.
     */
    ~PlaylistComponent() override;

    // Component methods
    /**
     * Paints the PlaylistComponent.
     *
     * @param g Graphics context used for drawing the component.
     */
    void paint(juce::Graphics&) override;

    /**
     * Resizes the PlaylistComponent and its child components.
     */
    void resized() override;

    // TableListBoxModel methods
    /**
     * Returns the number of rows in the playlist table.
     *
     * @return The number of tracks in the playlist.
     */
    int getNumRows() override;

    /**
     * Paints the background of a table row.
     *
     * @param g Graphics context used for drawing the row.
     * @param rowNumber The index of the row to paint.
     * @param width The width of the row.
     * @param height The height of the row.
     * @param rowIsSelected Whether the row is currently selected.
     */
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

    /**
     * Paints the content of a table cell.
     *
     * @param g Graphics context used for drawing the cell.
     * @param rowNumber The index of the row to paint.
     * @param columnId The ID of the column being painted.
     * @param width The width of the cell.
     * @param height The height of the cell.
     * @param rowIsSelected Whether the row is currently selected.
     */
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /**
     * Refreshes a component for each table cell, such as the buttons for loading onto decks.
     *
     * @param rowNumber The index of the row to refresh.
     * @param columnId The ID of the column being refreshed.
     * @param isRowSelected Whether the row is currently selected.
     * @param existingComponentToUpdate The existing component to update.
     *
     * @return A pointer to the component to be displayed in the cell.
     */
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    // Playlist management methods
    /**
     * Loads the playlist from a file.
     *
     * @param file The file from which to load the playlist.
     */
    void loadPlaylistFromFile(const File& file);

    /**
     * Saves the playlist to a file.
     *
     * @param file The file to which the playlist will be saved.
     */
    void savePlaylistToFile(const File& file);

    /**
     * Processes a file by adding it to the playlist.
     *
     * @param file The file to be processed.
     */
    void processFile(File file);
    
    TableListBox tableComponent;  // The table that displays the playlist
private:
    // Helper methods
    /**
     * Updates the search results based on the search query.
     */
    void updateSearchResults();

    /**
     * Searches the library for tracks that match the query.
     *
     * @param trackTitles The titles of the tracks to search through.
     */
    void searchLibrary(std::vector<std::string> trackTitles);

    /**
     * Loads an audio file and returns its length in a formatted string.
     *
     * @param file The file to be loaded.
     *
     * @return The formatted track length as a string.
     */
    String loadFileAndGetLength(File file);

    // Button::Listener method
    /**
     * Responds to button click events.
     *
     * @param button Pointer to the button that was clicked.
     */
    void buttonClicked(Button* button) override;

    // FileDragAndDropTarget methods
    /**
     * Returns whether the component is interested in a file drag event.
     *
     * @param files The list of files being dragged.
     *
     * @return True if the component is interested in the file drag event.
     */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /**
     * Responds to files being dropped onto the component.
     *
     * @param files The list of files that were dropped.
     * @param x The x-coordinate of the drop position.
     * @param y The y-coordinate of the drop position.
     */
    void filesDropped(const StringArray& files, int x, int y) override;

    // Members
    DJAudioPlayer* player1;  // Pointer to the first DJAudioPlayer
    DJAudioPlayer* player2;  // Pointer to the second DJAudioPlayer
    WaveformDisplay* waveformDisplay;  // Pointer to the first WaveformDisplay
    WaveformDisplay* waveformDisplay2;  // Pointer to the second WaveformDisplay

    std::vector<std::string> trackTitles;  // Track titles in the playlist
    std::vector<std::string> trackLengths;  // Track lengths in the playlist
    Array<URL> fileURL;  // URLs (file paths) of the tracks in the playlist

    std::vector<std::string> filteredTrackTitles;  // Filtered track titles (search results)
    std::vector<std::string> filteredTrackLengths;  // Filtered track lengths (search results)
    Array<URL> filteredFileURL;  // Filtered file URLs (search results)

    TextEditor searchBox;  // The search box for filtering tracks
    ModernLookAndFeel modernLookAndFeel;  // Custom LookAndFeel for styling the component

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
