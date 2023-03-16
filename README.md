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

Choose one of the following releases.

* Use the [latest-qt5](https://github.com/micahmo/obs-progress/releases/tag/latest-qt5) release for OBS < 28.0.
* Use the [latest-qt6](https://github.com/micahmo/obs-progress/releases/tag/latest-qt6) release for OBS >= 28.0.

From the release, download `obs-progress.dll`. Place the file in OBS's 64-bit Plugins directory (for example, `C:\Program Files\obs-studio\obs-plugins\64bit`).

Start OBS to use. The progress may initially appear undocked. It can be repositioned manually and its size/position/state will be restored from session to session.

# FAQ

#### OBS added Media Controls to [OBS Studio 26.0](https://obsproject.com/forum/threads/obs-studio-26-0-release-candidate.129075/). Why is an additional plugin needed?
The built-in media controls are *very* limited. Namely, the controls are only shown for the currently selected source. This has several drawbacks.
- You must manually select the source before seeing the media controls.
- If there are multiple media sources in a given scene, you can only see the media controls for one at a time.
- If you are using Studio Mode, the Preview and Program windows will be different, so the selected source will never be the active one.

#### Why not use the existing [Media Controls](https://obsproject.com/forum/resources/media-controls.1032/) plugin?
Please do! It is a great plugin with good support. In fact, its code was incorporated into the official feature in OBS.

# OBS Studio Issues

There are no known issues with the latest versions of OBS.

# Development

Install Visual Studio with the "Desktop development with C++" option enabled.

Download and run the [CMake Windows x64 Installer](https://cmake.org/download/).

Clone [vcpkg](https://vcpkg.io/en/getting-started.html). Initialize with `.\bootstrap-vcpkg.bat`. Run `.\vcpkg.exe install date --triplet x64-windows` and `.\vcpkg integrate install`.

Clone the [OBS Studio](https://github.com/obsproject/obs-studio) repo (with `--recursive`). Run `CI/build-windows.ps1` to do a Release build and `CI/build-windows.ps1 -BuildConfiguration Debug` to do a Debug build.

Clone this repository. Update the paths in following command to reference your cloned repositories. Then navigate to the root of this repo and run it.

``` powershell
cmake . -B build `
-D LibObs_DIR="<your repos>\obs-studio\build64\libobs\" `
-D w32-pthreads_DIR="<your repos>\obs-studio\build64\deps\w32-pthreads\" `
-D OBS_FRONTEND_LIB="<your repos>\obs-studio\build64\UI\obs-frontend-api\Debug\obs-frontend-api.lib" `
-D OBS_LIBOBS_INCLUDE="<your repos>\obs-studio\libobs\" `
-D OBS_FRONTEND_INCLUDE="<your repos>\obs-studio\UI\obs-frontend-api\" `
-D OBS_LIB_DIR="<your repos>\obs-studio\build64\libobs\Debug\obs.lib" `
-D date_DIR="<your repos>\vcpkg\installed\x64-windows\share\date\" `
-D Qt6_DIR="<your repos>\obs-build-dependencies\windows-deps-2023-03-04-x64\lib\cmake\Qt6\" `
-D Qt6Core_DIR="<your repos>\obs-build-dependencies\windows-deps-2023-03-04-x64\lib\cmake\Qt6Core\" `
-D Qt6Widgets_DIR="<your repos>\obs-build-dependencies\windows-deps-2023-03-04-x64\lib\cmake\Qt6Widgets\" `
-D Qt6WidgetsTools_DIR="<your repos>\obs-build-dependencies\windows-deps-2023-03-04-x64\lib\cmake\Qt6WidgetsTools\" `
-D Qt6CoreTools_DIR="<your repos>\obs-build-dependencies\windows-deps-2023-03-04-x64\lib\cmake\Qt6CoreTools\" `
-D Qt6GuiTools_DIR="<your repos>\obs-build-dependencies\windows-deps-2023-03-04-x64\lib\cmake\Qt6GuiTools\"
```

Open the resulting `build\obs-progress.sln` in Visual Studio.

### Debug

Build the solution with the `Debug` configuration selected. Copy the output from `build\Debug\obs-progress.dll` to `obs-studio\build64\rundir\Debug\obs-plugins\64bit`.

Run the debug version of OBS from `obs-studio\build64\rundir\Debug\bin\64bit\obs64.exe`. It should then be possible to attach from Visual Studio to the running OBS to debug the plugin.

### Release

Build the solution with the `Release` configuration selected. Copy the output from `build\Release\obs-progress.dll` to `C:\Program Files\obs-studio\obs-plugins\64bit`.

Run OBS from `C:\Program Files\obs-studio\bin\64bit\obs64.exe` for final testing.
