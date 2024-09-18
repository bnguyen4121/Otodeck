# DJ Application

This is a simple DJ application built using the JUCE framework. It features two decks that allow users to load and mix audio tracks, complete with customizable playback controls, real-time waveform visualization, cue points, and a zoom slider for detailed track navigation.

## Features

- **Two Decks**: Play and mix two audio tracks simultaneously.
- **Waveform Display**: Real-time visualization of the waveform for each track, with dynamic color changes based on frequency content.
- **Cue Points**: Set and jump between cue points to easily navigate tracks during playback.
- **Zoom Slider**: Control the zoom level of the waveform to focus on specific sections of a track.
- **Playlist Management**: Load multiple audio files into a playlist and easily assign them to either deck.
- **Playback Control**: Adjust volume, speed, and position for each track independently.

## Screenshots
*(Add screenshots of your DJ application here to showcase its UI)*

## Installation

### Prerequisites

- **JUCE Framework**: You will need JUCE installed in your project directory to compile this project. You can download JUCE [here](https://juce.com/download).
- **CMake**: This project uses CMake for building. You can download CMake [here](https://cmake.org/download/).

### Build Instructions

1. Clone this repository:
    ```bash
    git clone https://github.com/your-username/dj-application.git
    cd dj-application
    ```

2. Generate the build files using CMake:
    ```bash
    cmake -B build
    ```

3. Build the project:
    ```bash
    cmake --build build
    ```

4. Run the application:
    ```bash
    ./build/DJApplication
    ```

## Usage

- **Loading Tracks**: Use the load button in each deck to load audio files from the playlist into the respective deck.
- **Playback Controls**: Start and stop playback using the buttons, adjust the volume and speed sliders, and use the position slider to seek through the track.
- **Cue Points**: Set up to four cue points per deck, allowing you to quickly return to specific points in the track.
- **Zoom Slider**: Use the zoom slider to zoom in or out of the waveform for more detailed control over cue point placement and track navigation.

## Project Structure

