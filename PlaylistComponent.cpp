//ALL CODE WRITTEN BY ME EXCEPT FOR THOSE WRITTEN BY DR. YEE-KING


#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <vector>
#include <string>
#include "WaveformDisplay.h"
#include "DJAudioPlayer.h"
#include "ModernLookAndFeel.h"

//==============================================================================
// Constructor: Initializes the PlaylistComponent with two audio players, waveform displays, and the format/cache for audio files.
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1,
                                     DJAudioPlayer* _player2,
                                     AudioFormatManager& formatManagerToUse,
                                     AudioThumbnailCache& cacheToUse,
                                     WaveformDisplay* _waveformDisplay,
                                     WaveformDisplay* _waveformDisplay2)
    : player1(_player1), player2(_player2), waveformDisplay(_waveformDisplay), waveformDisplay2(_waveformDisplay2)
{
    // Configure the search box
    addAndMakeVisible(searchBox);
    searchBox.setTextToShowWhenEmpty("Search for tracks...", Colours::grey); // Default text when empty
    searchBox.addListener(this);  // Attach listener to the search box
    searchBox.onTextChange = [this] { updateSearchResults(); }; // Update the search results when the text changes

    // Set up the table headers and define column structure
    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("LOAD ON TO LEFT DECK", 1, 350);  // Column 1 for loading onto the left deck
    tableComponent.getHeader().addColumn("TITLE", 2, 400);  // Column 2 for track titles
    tableComponent.getHeader().addColumn("TIME", 3, 100);  // Column 3 for track lengths
    tableComponent.getHeader().addColumn("LOAD ON TO RIGHT DECK", 4, 350);  // Column 4 for loading onto the right deck

    // Set the model of the table to be this class (this class provides row data)
    tableComponent.setModel(this);

    // Load the playlist from the file on start
    File saveFile = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("playlist.json");
    loadPlaylistFromFile(saveFile);
}

// Destructor: Cleans up any dynamic components and detaches listeners
PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr);  // Detach the model from the table component
    setLookAndFeel(nullptr);  // Reset any custom look and feel applied to the component
}

//==============================================================================
// Component painting function: Draws background and outline for the component
void PlaylistComponent::paint(juce::Graphics& g)
{
    // Fill the background with the window's background color
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Set color and draw an outline around the component
    g.setColour(juce::Colours::mintcream);
    g.drawRect(getLocalBounds(), 1);

    // Set text color and font, then draw placeholder text
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("PlaylistComponent", getLocalBounds(),
               juce::Justification::centred, true);
}

// Component resizing function: Adjusts the size and layout of child components
void PlaylistComponent::resized()
{
    // Set bounds for the search box and table
    searchBox.setBounds(0, 0, getWidth(), 30);  // Search box at the top
    tableComponent.setBounds(0, 30, getWidth(), getHeight() - 30);  // Table below the search box
}

//==============================================================================
// TableModel methods: Provides data for the table

// Returns the number of rows in the table (based on the filtered track titles)
int PlaylistComponent::getNumRows()
{
    return filteredTrackTitles.size();  // Return the number of filtered tracks
}

// Paints the background of each row (highlight if selected, default otherwise)
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)  // If the row is selected, highlight it
    {
        g.fillAll(Colours::mintcream);
    }
    else  // Otherwise, set the default color
    {
        g.fillAll(Colours::teal);
    }
}

// Paints the content of each cell in the table
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 2)  // Title column
    {
        g.drawText(filteredTrackTitles[rowNumber], 1, 0, width, height,
                   Justification::centredLeft, true);  // Draw track title
    }
    else if (columnId == 3)  // Time column
    {
        g.drawText(filteredTrackLengths[rowNumber], 0, 2, width, height,
                   Justification::centredLeft);  // Draw track length
    }
}

// Provides custom buttons for specific table cells (load buttons for left and right decks)
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    // Create a load button for the left deck
    if (columnId == 1)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"load left"};  // Button for loading track onto left deck
            btn->setLookAndFeel(&modernLookAndFeel);  // Apply custom look and feel
            btn->setComponentID(std::to_string(rowNumber));  // Use row number as the component ID
            btn->addListener(this);  // Attach a listener for button clicks
            existingComponentToUpdate = btn;
        }
    }
    // Create a load button for the right deck
    else if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"load right"};  // Button for loading track onto right deck
            btn->setLookAndFeel(&modernLookAndFeel);  // Apply custom look and feel
            btn->setComponentID(std::to_string(rowNumber) + "right");  // Use row number + "right" as the component ID
            btn->addListener(this);  // Attach a listener for button clicks
            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;  // Return the button for the cell
}

//==============================================================================
// Button click handler: Loads the selected track onto the left or right deck
void PlaylistComponent::buttonClicked(Button* button)
{
    String id = button->getComponentID();  // Get the component ID to identify the row and deck
    int rowNumber = id.getIntValue();  // Convert the component ID to a row number
    URL url;

    if (id.contains("right"))  // Check if the button is for the right deck
    {
        url = filteredFileURL[rowNumber];  // Get the URL of the selected track
        player2->loadURL(url);  // Load the track into player2 (right deck)
        waveformDisplay2->loadURL(url);  // Update the waveform for the right deck
    }
    else  // Otherwise, load the track into the left deck
    {
        url = filteredFileURL[rowNumber];  // Get the URL of the selected track
        player1->loadURL(url);  // Load the track into player1 (left deck)
        waveformDisplay->loadURL(url);  // Update the waveform for the left deck
    }
}

//==============================================================================
// Utility functions: Manage file loading, playlist saving/loading, and search

// Loads the file and returns its formatted length (e.g., "3:45")
String PlaylistComponent::loadFileAndGetLength(File file)
{
    player1->loadURL(URL{file});  // Load the file into player1
    double lengthInSeconds = player1->getLength();  // Get the length of the track

    // Convert the length from seconds to minutes and seconds
    int minutes = static_cast<int>(lengthInSeconds) / 60;
    int seconds = static_cast<int>(lengthInSeconds) % 60;

    // Format and return the track length
    return String(minutes) + ":" + String(seconds).paddedLeft('0', 2);
}

// Processes the selected file and adds it to the playlist
void PlaylistComponent::processFile(File file)
{
    trackTitles.push_back(file.getFileName().toStdString());  // Add the file name to the track titles
    fileURL.add(URL{file});  // Add the file URL to the list
    String trackLength = loadFileAndGetLength(file);  // Get the track's length
    trackLengths.push_back(trackLength.toStdString());  // Add the track length to the list

    updateSearchResults();  // Refresh the search results to include the new track
}

//==============================================================================
// File drag-and-drop methods

// Returns true if the component is interested in file drag events
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;  // Always interested in file drags
}

// Handles files dropped onto the component
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    std::cout << "PlaylistComponent::filesDropped" << std::endl;

    // Process each file that was dropped
    for (const auto& filePath : files)
    {
        File file{filePath};
        processFile(file);  // Add the file to the playlist
    }

    updateSearchResults();  // Refresh the table to display the new files
}

//==============================================================================
// Playlist persistence: Saving and loading the playlist to/from a JSON file

// Saves the playlist to the specified file in JSON format
void PlaylistComponent::savePlaylistToFile(const File& file)
{
    auto playlistJson = new juce::DynamicObject();  // Create a dynamic object to represent the playlist
    juce::Array<var> tracksArray;  // Create an array to hold the tracks

    // Populate the array with each track's details (title, URL, length)
    for (size_t i = 0; i < trackTitles.size(); ++i)
    {
        auto trackJson = new juce::DynamicObject();
        trackJson->setProperty("title", juce::String(trackTitles[i]));
        trackJson->setProperty("url", fileURL[i].toString(true));
        trackJson->setProperty("length", juce::String(trackLengths[i]));
        tracksArray.add(var(trackJson));
    }

    playlistJson->setProperty("tracks", tracksArray);  // Add the tracks array to the main object

    juce::FileOutputStream outputStream(file);  // Open the file for writing
    if (outputStream.openedOk())  // Check if the file was opened successfully
    {
        outputStream.setPosition(0);  // Move to the beginning of the file
        outputStream.truncate();  // Remove old content
        var json(playlistJson);  // Convert the dynamic object to JSON format
        juce::JSON::writeToStream(outputStream, json);  // Write the JSON to the file
    }
}

// Loads the playlist from the specified JSON file
void PlaylistComponent::loadPlaylistFromFile(const File& file)
{
    juce::FileInputStream inputStream(file);  // Open the file for reading
    if (inputStream.openedOk())  // Check if the file was opened successfully
    {
        var json = juce::JSON::parse(inputStream);  // Parse the JSON file
        if (auto* jsonObject = json.getDynamicObject())  // Check if the JSON object is valid
        {
            auto tracksArray = jsonObject->getProperty("tracks").getArray();  // Get the tracks array
            if (tracksArray != nullptr)
            {
                trackTitles.clear();
                fileURL.clear();
                trackLengths.clear();

                // Populate the playlist with the data from the JSON file
                for (auto& trackVar : *tracksArray)
                {
                    auto* trackJson = trackVar.getDynamicObject();
                    if (trackJson != nullptr)
                    {
                        trackTitles.push_back(trackJson->getProperty("title").toString().toStdString());
                        fileURL.add(URL(trackJson->getProperty("url").toString()));
                        trackLengths.push_back(trackJson->getProperty("length").toString().toStdString());
                    }
                }

                updateSearchResults();  // Refresh the filtered lists
            }
        }
    }
}

//==============================================================================
// Search function: Filters the playlist based on the search query
void PlaylistComponent::updateSearchResults()
{
    String searchQuery = searchBox.getText();  // Get the search query
    filteredTrackTitles.clear();
    filteredTrackLengths.clear();
    filteredFileURL.clear();

    // If the search query is empty, show all tracks
    if (searchQuery.isEmpty())
    {
        filteredTrackTitles = trackTitles;
        filteredTrackLengths = trackLengths;
        filteredFileURL = fileURL;
    }
    else
    {
        // Filter the tracks by the search query (case-insensitive)
        for (size_t i = 0; i < trackTitles.size(); ++i)
        {
            if (juce::String(trackTitles[i]).containsIgnoreCase(searchQuery))
            {
                filteredTrackTitles.push_back(trackTitles[i]);
                filteredTrackLengths.push_back(trackLengths[i]);
                filteredFileURL.add(fileURL[i]);
            }
        }
    }

    tableComponent.updateContent();  // Refresh the table to display the filtered results
}
