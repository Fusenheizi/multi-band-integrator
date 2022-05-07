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
/*
This is based heavily on the crossing detector third party plugin
First, simply filtering the data before thresholding
*/

#include "MultiBandIntegrator.h"

#include "MultiBandIntegratorEditor.h"


MultiBandIntegratorSettings::MultiBandIntegratorSettings() :
    localChannelIndex(0),
    alphaGain(4.0f),
    betaGain(7.0f),
    deltaGain(-1.0f)
{
    
    for (int n = 0; n < 3; n = n + 1)
    {
        filters.add(new Dsp::SmoothedFilterDesign
            <Dsp::Butterworth::Design::BandPass    // design type
            <2>,                                   // order
            1,                                     // number of channels (must be const)
            Dsp::DirectFormII>(1));               // realization
    }

}

void MultiBandIntegratorSettings::updateFilter(int index, float sampleRate,
                                                var lowCut,
                                                var highCut)
{
    
    Dsp::Params params;
    params[0] = sampleRate;                               // sample rate
    params[1] = 2;                                        // order
    params[2] = (float(highCut) + float(lowCut)) / 2;     // center frequency
    params[3] = float(highCut) - float(lowCut);           // bandwidth

    filters[index]->setParams(params);

}

void MultiBandIntegratorSettings::setRollingWindowParameters(float sampleRate, var rollDuration)
{

    int buffSize = int(sampleRate * float(rollDuration) / 1000.0f);

    rollingAverage.setSize(buffSize);

}


MultiBandIntegrator::MultiBandIntegrator()
    : GenericProcessor  ("Multi-band Integrator")
{
    
    addSelectedChannelsParameter(Parameter::STREAM_SCOPE,
                                 "Channel", "The input channel to analyze", 1);
    
    
    addIntParameter(Parameter::GLOBAL_SCOPE,
                    "window_ms", "The size of the rolling average window in milliseconds",
                    1000, 10, 5000);
    
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "alpha_low", "The alpha band low cut",
                    6.0, 0.1, 100.0, false);
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "alpha_high", "The alpha band high cut",
                    9.0, 0.1, 100.0, false);
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "alpha_gain", "The alpha band gain",
                    4.0, -10.0, 10.0, false);
    
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "beta_low", "The beta band low cut",
                    13.0, 0.1, 100.0, false);
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "beta_high", "The beta band high cut",
                    18.0, 0.1, 100.0, false);
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "beta_gain", "The beta band gain",
                    7.0, -10.0, 10.0, false);
    
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "delta_low", "The delta band low cut",
                    1.0, 0.1, 100.0, false);
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "delta_high", "The delta band high cut",
                    4.0, 0.1, 100.0, false);
    addFloatParameter(Parameter::GLOBAL_SCOPE,
                    "delta_gain", "The delta band gain",
                    -1.0, -10.0, 10.0, false);
    
    scratchBuffer.setSize(3, 10000);
}

AudioProcessorEditor* MultiBandIntegrator::createEditor()
{
    editor = std::make_unique<MultiBandIntegratorEditor> (this);

    return editor.get();
}


void MultiBandIntegrator::updateSettings()
{
    
    settings.update(getDataStreams());
    
    for (auto stream : getDataStreams())
    {
        for (int i = 0; i < 3; i++)
        {
            
            var lowCut, highCut;
            
            if (i == 0)
            {
                lowCut = getParameter("alpha_low")->getValue();
                highCut = getParameter("alpha_high")->getValue();
            } else if (i == 1)
            {
                lowCut = getParameter("beta_low")->getValue();
                highCut = getParameter("beta_high")->getValue();
            } else if (i == 2)
            {
                lowCut = getParameter("delta_low")->getValue();
                highCut = getParameter("delta_high")->getValue();
            }
            
            settings[stream->getStreamId()]->updateFilter(i,
                                                           stream->getSampleRate(),
                                                          lowCut, highCut);

        }
        
        settings[stream->getStreamId()]->setRollingWindowParameters(stream->getSampleRate(),
                                                                    getParameter("window_ms")->getValue());
        
    }
}

void MultiBandIntegrator::process(AudioBuffer<float>& continuousBuffer)
{
    
    for (auto stream : getDataStreams())
    {

        if ((*stream)["enable_stream"])
        {
            MultiBandIntegratorSettings* module = settings[stream->getStreamId()];
            
            const uint16 streamId = stream->getStreamId();
            const uint32 numSamplesInBlock = getNumSamplesInBlock(streamId);

            int localIndex = module->localChannelIndex;
            
            if (localIndex < 0)
                continue;
            
            int globalIndex = stream->getContinuousChannels()[localIndex]->getGlobalIndex();
            
            for (int i = 0; i < 3; i++)
            {
                scratchBuffer.copyFrom(i,
                                       0,
                                       continuousBuffer,
                                       globalIndex,
                                       0,
                                       numSamplesInBlock);
            }
            
            float* ptrA = scratchBuffer.getWritePointer(0);
            module->filters[0]->process(numSamplesInBlock, &ptrA);
            scratchBuffer.applyGain(0,
                                    0,
                                    numSamplesInBlock,
                                    module->alphaGain);

            float* ptrB = scratchBuffer.getWritePointer(1);
            module->filters[1]->process(numSamplesInBlock, &ptrB);
            scratchBuffer.applyGain(1,
                                    0,
                                    numSamplesInBlock,
                                    module->betaGain);

            float* ptrD = scratchBuffer.getWritePointer(2);
            module->filters[2]->process(numSamplesInBlock, &ptrD);
            scratchBuffer.applyGain(2,
                                    0,
                                    numSamplesInBlock,
                                    module->deltaGain);

            //Now sum the samples together into channel 0 of the scratch buffer
            scratchBuffer.addFrom(0,                  //dest channel
                                 0,                   //dest start sample
                                 scratchBuffer,       //source buffer
                                 1,                   //source channel
                                 0,                   //source start sample
                                 numSamplesInBlock);  //num samples

            scratchBuffer.addFrom(0,                  //dest channel
                                 0,                   //dest start sample
                                 scratchBuffer,       //source buffer
                                 2,                   //source channel
                                 0,                   //source start sample
                                 numSamplesInBlock);  //num samples
            
            //put the rolling mean into channel 1 of the scratch buffer
            scratchBuffer.setSample(1, 0, module->rollingAverage.calculate());

            for (int i = 0; i < numSamplesInBlock-1; i++)
            {
                
                module->rollingAverage.addSample(
                        std::fabs(scratchBuffer.getSample(0, i + 1)
                                - scratchBuffer.getSample(0, i)));
                
                
                scratchBuffer.setSample(1, i+1, module->rollingAverage.calculate());
            }

            //add gain to output signal so that its units are more useful
            scratchBuffer.applyGain(1, 0, numSamplesInBlock, 10);

            //overwrite the input channel with averaged data
            continuousBuffer.copyFrom(globalIndex,
                                      0,
                                      scratchBuffer,
                                      1,
                                      0,
                                      numSamplesInBlock);
        }
    }
}

void MultiBandIntegrator::parameterValueChanged(Parameter* param)
{
    if (param->getName().equalsIgnoreCase("alpha_low"))
    {

        if (param->getValue() >= getParameter("alpha_high")->getValue())
        {
            param->restorePreviousValue();
            return;
        }

        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->updateFilter(0,
                                                          stream->getSampleRate(),
                                                          param->getValue(),
                                                          getParameter("alpha_high")->getValue());
        }
    }
    else if (param->getName().equalsIgnoreCase("alpha_high"))
    {

        if (param->getValue() <= getParameter("alpha_low")->getValue())
        {
            param->restorePreviousValue();
            return;
        }

        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->updateFilter(0,
                                                          stream->getSampleRate(),
                                                          getParameter("alpha_low")->getValue(),
                                                          param->getValue());
        }
    } else if (param->getName().equalsIgnoreCase("alpha_gain"))
    {
        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->alphaGain = float(param->getValue());
        }
    } if (param->getName().equalsIgnoreCase("beta_low"))
    {

        if (param->getValue() >= getParameter("beta_high")->getValue())
        {
            param->restorePreviousValue();
            return;
        }

        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->updateFilter(1,
                                                          stream->getSampleRate(),
                                                          param->getValue(),
                                                          getParameter("beta_high")->getValue());
        }
    }
    else if (param->getName().equalsIgnoreCase("beta_high"))
    {

        if (param->getValue() <= getParameter("beta_low")->getValue())
        {
            param->restorePreviousValue();
            return;
        }

        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->updateFilter(1,
                                                          stream->getSampleRate(),
                                                          getParameter("beta_low")->getValue(),
                                                          param->getValue());
        }
    } else if (param->getName().equalsIgnoreCase("beta_gain"))
    {
        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->betaGain = float(param->getValue());
        }
    }  if (param->getName().equalsIgnoreCase("delta_low"))
    {

        if (param->getValue() >= getParameter("delta_high")->getValue())
        {
            param->restorePreviousValue();
            return;
        }

        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->updateFilter(2,
                                                          stream->getSampleRate(),
                                                          param->getValue(),
                                                          getParameter("delta_high")->getValue());
        }
    }
    else if (param->getName().equalsIgnoreCase("delta_high"))
    {

        if (param->getValue() <= getParameter("delta_low")->getValue())
        {
            param->restorePreviousValue();
            return;
        }

        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->updateFilter(2,
                                                          stream->getSampleRate(),
                                                          getParameter("delta_low")->getValue(),
                                                          param->getValue());
        }
    } else if (param->getName().equalsIgnoreCase("delta_gain"))
    {
        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->deltaGain = float(param->getValue());
        }
    }  else if (param->getName().equalsIgnoreCase("window_ms"))
    {
        for (auto stream : getDataStreams())
        {
            settings[stream->getStreamId()]->setRollingWindowParameters(stream->getSampleRate(), param->getValue());
        }
    } else if (param->getName().equalsIgnoreCase("Channel"))
    {
        Array<var>* array = param->getValue().getArray();
        
        if (array->size() > 0)
            settings[param->getStreamId()]->localChannelIndex = int(array->getReference(0));
        else
            settings[param->getStreamId()]->localChannelIndex = -1;
    }
}

RollingAverage::RollingAverage()
{
	setSize(1);

	newSamples = 0;
	sum = 0;
}

void RollingAverage::setSize(int numSamples)
{
	buffer.clear();
	buffer.insertMultiple(0, 0, numSamples);
	index = 0;

	sum = 0;
}

void RollingAverage::addSample(double sample)
{
	sum -= buffer[index];
	sum += sample;

	buffer.set(index, sample);

	index += 1;
	index %= buffer.size();
}


double RollingAverage::calculate() {

	return sum / buffer.size();
}
