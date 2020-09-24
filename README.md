# obs-progress

A plugin for OBS Studio that provides playback controls and progress indicators for media and slideshow sources. The controls are available in a standard OBS dock that can be floated, pinned, hidden, etc.

![screenshot](https://i.imgur.com/CTxWdmG.png)

The following media indicators and controls are shown for all applicable sources in the active scene.
- Media
  - Play/pause/stop
  - Toggle loop option
  - Click progress bar or click and drag playhead to seek through media
  - Elapsed/total/remaining time
- Slideshows
  - Next/previous/restart

![screenshot](https://i.imgur.com/y1iwCVY.png)

Here are the controls pinned to the UI. The title of the dock displays the scene name and the text above the controls shows the source name. In this case, the active scene contains a slideshow and a media source.

# How to Use

Download the latest version of [obs-progress.dll](https://github.com/micahmo/obs-progress/releases/download/latest/obs-progress.dll).

Place the file in OBS's 64-bit Plugins directory (for example, `C:\Program Files\obs-studio\obs-plugins\64bit`).

Start OBS to use. The progress dock will initially appear undocked at the top-left. It can be repositioned manualy and its size/position/state will be restored from session to session.
