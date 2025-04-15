# Qt YouTube Player

A modern YouTube video player built with Qt 6 and libmpv. This application allows you to play YouTube videos and local video files with a clean and intuitive interface.

## Features

- YouTube video playback support
- Local video file playback
- Basic playback controls (play, pause, stop)
- Video seeking with slider
- Time display
- OpenGL-based video rendering

## Dependencies

- Qt 6 (Core, Gui, Widgets, Network, Multimedia, MultimediaWidgets, OpenGLWidgets)
- libmpv
- youtube-dl or yt-dlp (for YouTube video support)

## Building

1. Make sure you have all dependencies installed
2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
3. Run CMake:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   make
   ```

## Usage

1. Launch the application
2. Enter a YouTube URL or local video file path in the text field
3. Click "Play" to start playback
4. Use the controls at the bottom to:
   - Stop playback
   - Pause/Resume
   - Seek through the video using the slider
   - Monitor playback time

## License

MIT License 