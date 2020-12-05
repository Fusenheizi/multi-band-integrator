# Multi-Band Integrator

This Open Ephys GUI plugin allows the user to mix, weight, and apply a rolling average to bandwidths of interest on a single channel.  Its output can be thought of as a real-time power signal for waveforms with complex but well-specified frequency components.  It can be paired with the crossing detector plugin to trigger events when the output power reaches a threshold, to trigger events based on waveforms of interest.

It was originally developed by Michelle Fogerson in the Huguenard Lab at Stanford to detect absence-like seizures in mice in real time based on their spectral properties [(Sorokin et al., 2016)](https://www.sciencedirect.com/science/article/abs/pii/S0928425717300372).

## Settings
Users can specify 
* input channel 
* rolling average window duration
* Up to 3 frequency bands
* Gain for each frequency band

## Installation

Place the source code in a directory inside a plugin-specific directory at the same level as the "plugin-GUI"

e.g.:
```
/code
    /plugin-GUI
    /OEPlugins
        /multi-band-integrator
            /Source
            /Build
```

From inside the "Build" directory, run:

```
> cmake -G "Visual Studio 16 2019" -A x64 ..
```

Then, open the generated .sln file to compile / install.

This plugin has only been built and tested on Windows so far. It depends on the Dsp library (included in the source directory).