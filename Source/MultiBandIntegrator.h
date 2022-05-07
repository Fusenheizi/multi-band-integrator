/*
------------------------------------------------------------------

This file is part of a plugin for the Open Ephys GUI
Copyright (C) 2017 Translational NeuroEngineering Laboratory, MGH

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


/* The multi-band integrator allows the user to take a weighted sum of up to 3 frequency bands and apply a rolling average to build a power signal for complex waveforms with well-defined spectral properties. It was initially developed to detect absence-like seizures in real time from EEG recorded in awake, head-fixed mice. The user sets the duration of the rolling window and input channel, as well as frequency ranges and gains that define the waveform of interest. The processed signal is output on the same channel as the input channel. Two adjacent channels are overwritten with the initial raw input channel and the weighted summed signal before averaging to allow the user to fine-tune frequency ranges and gains on a per-animal basis.
 
 Because channels are overwritten, I recommend using a split path in Open Ephys. One of the split paths includes the seizure detector followed by an LFP viewer to show how the input channel is being filtered. The other contains a second LFP viewer to show all of the channels without any multi-band integrator processing.
 
 This plugin can be used with the third party crossing detector plugin to trigger events based on the processed output from the multi-band integrator.*/


#ifndef MULTIBAND_INTEGRATOR_H_INCLUDED
#define MULTIBAND_INTEGRATOR_H_INCLUDED

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>
#include <algorithm> // max



/**
    Computes the rolling average of a signal.
 */
class RollingAverage
{
public:
    
    /** Constructor */
	RollingAverage();
    
    /** Destructor */
    ~RollingAverage() { }

    /** Sets the size of the buffer */
	void setSize(int numSamples);
    
    /** Adds a sample to the buffer*/
	void addSample(double sample);
    
    /** Returns the average of the current buffer*/
	double calculate();

private:

	Array<double> buffer;
	int index;

	double sum;
	int newSamples;
	double lastAvg;
};


/** Holds settings for one stream's multi-band integrator */
class MultiBandIntegratorSettings
{
public:
    /** Constructor -- sets default values*/
    MultiBandIntegratorSettings();

    /** Destructor*/
    ~MultiBandIntegratorSettings() { }
    
    /** Updates filter parameters */
    void updateFilter(int index,
                      float sampleRate,
                      var lowCut,
                      var highCut);

    /** Updates rolling window parameters*/
    void setRollingWindowParameters(float sampleRate, var durationMs);

    OwnedArray<Dsp::Filter> filters;
    
    RollingAverage rollingAverage;

    int localChannelIndex;
    
    float alphaGain;
    float betaGain;
    float deltaGain;
};

/**
 
 Computes a weighted sum of up to 3 frequency bands and applies a rolling average to build a power signal
 for complex waveforms with well-defined spectral properties.
 
 It was initially developed to detect absence-like seizures in real time from EEG recorded in awake, head-fixed mice.
 
 The user sets the duration of the rolling as well as frequency ranges and gains that define
 the waveform of interest. The processed signals are output on the input channels that have been selected.
 
 */
class MultiBandIntegrator : public GenericProcessor
{
    friend class MultiBandIntegratorEditor;

public:
    
    /** Constructor */
    MultiBandIntegrator();
    
    /** Destructor */
    ~MultiBandIntegrator() { }
    
    /** Creates the custom editor for this plugin */
    AudioProcessorEditor* createEditor() override;
    
    /** Applies filters to a subset of channels, and sums the resulst*/
    void process(AudioBuffer<float>& buffer) override;

    /** Called whenever the settings of upstream plugins change */
	void updateSettings() override;

    /** Called whenever a parameter's value is changed (called by GenericProcessor::setParameter())*/
    void parameterValueChanged(Parameter* param) override;

private:
    
    StreamSettings<MultiBandIntegratorSettings> settings;

    AudioBuffer<float> scratchBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiBandIntegrator);
};

#endif 
