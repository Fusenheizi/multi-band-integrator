# Multi-Band Integrator

![multi-band-integrator-screenshot](Resources/multi-band-integrator.png)

Mixes, weights, and applies a rolling average to three bandwidths of interest on each continuous input channel. The Multi-Band Integrator can be used to detect many types of neural activity with well-specified frequency components, such as seizures, ripples, and alpha waves. It is usually paired with the [Crossing Detector](https://github.com/open-ephys-plugins/crossing-detector) to trigger events when the integrated power crosses a particular threshold.


## Installation

(Coming soon)


## Usage

(link to documentation)


## Building from source

First, follow the instructions on [this page](https://open-ephys.github.io/gui-docs/Developer-Guide/Compiling-the-GUI.html) to build the Open Ephys GUI.

**Important:** This plugin is intended for use with the pre-release core application, version 0.6.0. The GUI should be compiled from the [`development-juce6`](https://github.com/open-ephys/plugin-gui/tree/development-juce6) branch, rather than the `master` branch.

Then, clone this repository into a directory at the same level as the `plugin-GUI`, e.g.:
 
```
Code
├── plugin-GUI
│   ├── Build
│   ├── Source
│   └── ...
├── OEPlugins
│   └── multi-band-integrator
│       ├── Build
│       ├── Source
│       └── ...
```

### Windows

**Requirements:** [Visual Studio](https://visualstudio.microsoft.com/) and [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
```

Next, launch Visual Studio and open the `OE_PLUGIN_multi-band-integrator.sln` file that was just created. Select the appropriate configuration (Debug/Release) and build the solution.

Selecting the `INSTALL` project and manually building it will copy the `.dll` and any other required files into the GUI's `plugins` directory. The next time you launch the GUI from Visual Studio, the Multi-Band Integrator plugin should be available.


### Linux

**Requirements:** [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Unix Makefiles" ..
cd Debug
make -j
make install
```

This will build the plugin and copy the `.so` file into the GUI's `plugins` directory. The next time you launch the GUI compiled version of the GUI, the Multi-Band Integrator plugin should be available.


### macOS

**Requirements:** [Xcode](https://developer.apple.com/xcode/) and [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Xcode" ..
```

Next, launch Xcode and open the `multi-band-integrator.xcodeproj` file that now lives in the “Build” directory.

Running the `ALL_BUILD` scheme will compile the plugin; running the `INSTALL` scheme will install the `.bundle` file to `/Users/<username>/Library/Application Support/open-ephys/plugins-api`. The Multi-Band Integrator plugin should be available the next time you launch the GUI from Xcode.

## Attribution

This plugin was originally developed by Michelle Fogerson in the Huguenard Lab at Stanford to perform real-time detection of absence-like seizures in mice [(Sorokin et al., 2016)](https://www.sciencedirect.com/science/article/abs/pii/S0928425717300372). It is now maintained by the Allen Institute.
