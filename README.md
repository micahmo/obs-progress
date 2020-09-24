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

Start OBS to use. The progress dock will initially appear undocked at the top-left. It can be repositioned manually and its size/position/state will be restored from session to session.

# FAQ

#### OBS is adding Media Controls to [OBS Studio 26.0](https://obsproject.com/forum/threads/obs-studio-26-0-release-candidate.129075/). Why is an additional plugin needed?
The built-in media controls coming in OBS 26.0 are *very* limited. Namely, the controls are only shown for the currently selected source. This has several drawbacks.
- You must manually select the source before seeing the media controls.
- If there are multiple media sources in a given scene, you can't see the media controls for more than one at once.
- If you are using Studio Mode, you will *never* see the media controls, because the Preview and Program windows will always be different, so the selected source will never be the active one.

#### Why not use the existing [Media Controls](https://obsproject.com/forum/resources/media-controls.1032/) plugin?
Please do! It is a great plugin with good support. In fact, the code was incorporated as the basis for the built-in media controls feature in 26.0.

# OBS Studio Issues
I have experienced an issue with OBS Studio 25.0.8 where the slideshow butons are unable to control the slideshow source. This issue seems to be resolved in the [26.0-rc3 release](https://obsproject.com/forum/threads/obs-studio-26-0-release-candidate.129075/).
