# DJ Application

This is a simple DJ application built using the JUCE framework. It features two decks that allow users to load and mix audio tracks, complete with customizable playback controls, real-time waveform visualization, cue points, and a zoom slider for detailed track navigation.

## Features

- **Two Decks for Simultaneous Playback**:  
  The application includes two decks, each functioning independently. This allows users to load two audio tracks and play them simultaneously. Users can control playback for each deck, making it possible to mix tracks by adjusting volume, speed, and positioning.

- **Real-Time Waveform Visualization**:  
  A key feature of the application is the real-time visualization of the waveform. The waveform is color-coded based on frequency content, providing users with a visual representation of low, mid, and high-frequency sounds. This feature helps DJs better understand track dynamics and improve their mixing performance.

- **Cue Points**:  
  Users can set up to four cue points per deck. These cue points act as markers in the track, allowing users to quickly jump to specific moments. Each cue point is represented as a colored line on the waveform for easy identification. Users can remove or reset cue points during playback for added flexibility.

- **Zoom Slider for Waveform Navigation**:  
  The zoom slider allows users to zoom in and out on the waveform, providing a more detailed view of the track. This feature is particularly useful when users need to set precise cue points or analyze specific parts of the audio.

- **Volume, Speed, and Position Controls**:  
  Each deck has its own set of sliders to control playback speed, volume, and track position. The position slider allows users to seek through the track easily, while the speed slider adjusts playback speed from 0.25x to 5x. The volume slider adjusts the output volume for each deck, enabling smooth transitions between tracks.

- **Playlist Management**:  
  The application includes a simple playlist manager, allowing users to load multiple audio files at once. Tracks in the playlist can be assigned to either deck for playback. The search bar feature lets users quickly find tracks in large playlists.

- **Customizable Look and Feel**:  
  The application’s user interface has been designed with a modern look and feel, featuring custom button designs, color-coded waveforms, and intuitive sliders. The colors and layout provide a professional and sleek user experience.

## Screenshots
*(Add screenshots of your DJ application here to showcase its UI)*

## Usage

1. **Loading Tracks**:  
   To load a track into a deck, click the "Load" button for the desired deck. A file chooser will appear, allowing you to select audio files from your device. Once loaded, the track’s waveform will be displayed in the waveform area.

2. **Playback Controls**:  
   Each deck has "Play" and "Stop" buttons. Click "Play" to start playback of the loaded track, and "Stop" to pause it. The playhead will move across the waveform as the track plays, giving you a visual cue of where in the track you are.

3. **Setting Cue Points**:  
   While the track is playing or paused, you can set cue points by clicking one of the four cue buttons (1, 2, 3, 4) below the waveform. Each cue point is visually represented as a colored vertical line on the waveform. Once set, you can jump to a cue point by clicking the corresponding button again. To remove a cue point, click the cue button a second time.

4. **Adjusting Volume and Speed**:  
   - **Volume Slider**: Use the volume slider to adjust the output volume for each deck.
   - **Speed Slider**: Adjust the playback speed of the track. The default speed is 1x, but you can slow the track down to 0.25x or speed it up to 5x.
   - **Position Slider**: Drag the position slider to move forward or backward through the track.

5. **Zooming In and Out**:  
   The zoom slider allows you to zoom in on the waveform for more detailed control over cue points and track navigation. Zooming out will give you a full view of the track, while zooming in will focus on a smaller segment of the track for precise control.

6. **Managing the Playlist**:  
   The playlist allows you to load multiple audio files at once. You can assign tracks from the playlist to either deck by clicking the "Load Left" or "Load Right" button next to the track in the playlist. You can also search for tracks using the search bar at the top of the playlist.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Built using the [JUCE](https://juce.com/) framework.

