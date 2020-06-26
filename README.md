# obs-progress

A plugin for OBS Studio that shows progress of all media files in the active scene, including audio and video. A progress bar, along with elapsed time, total time, and remaining time, is shown for each media source. Progress is displayed in a standard OBS dock that can be floated, pinned, hidden, etc.

![screenshot](https://i.imgur.com/aKYiKww.png)

Here it is pinned above the Scene Transitions and Controls docks. The title of the dock displays the scene name, while the text above the prgress bar(s) show the source name.

# How to Use

Download the latest version of [obs-progress.dll](https://github.com/micahmo/obs-progress/releases/download/latest/obs-progress.dll).

Place the file in OBS's 64-bit Plugins directory (for example, `C:\Program Files\obs-studio\obs-plugins\64bit`).

Start OBS to use. The progress dock will initially appear undocked at the top-left. It can be repositioned manualy and its size/position/state will be restored from session to session.

# Issues

* ~~Currently, OBS cannot remember the dock position of plugins, because docks are added at each startup.~~ Fixed in [b908e60](https://github.com/micahmo/obs-progress/commit/b908e60f4300083ed6bc5c06f383ca94f23daedd).
* obs-progress plugin ocasionally causes crash on shutdown. Exception comes from Qt code.
