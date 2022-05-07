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

#include "MultiBandIntegratorEditor.h"
#include "MultiBandIntegrator.h"

CustomLabel::CustomLabel(Parameter* param, Colour colour) : ParameterEditor(param)
{
    label.addListener(this);
    label.setBounds(0, 0, 40, 15);
    label.setEditable(true);
    label.setColour(Label::backgroundColourId, colour);
    label.setColour(Label::textColourId, Colour(200,200,200));
    addAndMakeVisible(&label);
    setBounds(0, 0, 40, 15);
}


void BackgroundComponent::paint(Graphics& g)
{
    g.setColour(Colours::lightgrey);
    g.fillRoundedRectangle(115, 25, 130, 65, 3.0f);
    
    g.setColour(Colours::darkgrey);
    g.drawText("low", 120, 12, 40, 10, Justification::centred);
    g.drawText("high", 160, 12, 40, 10, Justification::centred);
    g.drawText("gain", 200, 12, 40, 10, Justification::centred);
    
    g.drawText(CharPointer_UTF8("α"), 103, 32, 50, 10, Justification::left);
    g.drawText(CharPointer_UTF8("β"), 103, 52, 50, 10, Justification::left);
    g.drawText(CharPointer_UTF8("δ"), 103, 72, 50, 10, Justification::left);
    
    
}

MultiBandIntegratorEditor::MultiBandIntegratorEditor(GenericProcessor* parentNode)
    : GenericEditor(parentNode)
{
	desiredWidth = 254;
    
    addAndMakeVisible(&backgroundComponent);
    backgroundComponent.setBounds(0, 25, 250, 140);

    addSelectedChannelsParameterEditor("Channel", 15, 43);
    addTextBoxParameterEditor("window_ms", 15, 74);
    
    Colour alphaColour = Colour(30,30,30);
    Colour betaColour = Colour(60,60,60);
    Colour deltaColour = Colour(90,90,90);
    
    Parameter* param = getProcessor()->getParameter("alpha_low");
    addCustomParameterEditor(new CustomLabel(param, alphaColour), 120, 55);
    param = getProcessor()->getParameter("alpha_high");
    addCustomParameterEditor(new CustomLabel(param, alphaColour), 160, 55);
    param = getProcessor()->getParameter("alpha_gain");
    addCustomParameterEditor(new CustomLabel(param, alphaColour), 200, 55);
    
    param = getProcessor()->getParameter("beta_low");
    addCustomParameterEditor(new CustomLabel(param, betaColour), 120, 75);
    param = getProcessor()->getParameter("beta_high");
    addCustomParameterEditor(new CustomLabel(param, betaColour), 160, 75);
    param = getProcessor()->getParameter("beta_gain");
    addCustomParameterEditor(new CustomLabel(param, betaColour), 200, 75);
    
    param = getProcessor()->getParameter("delta_low");
    addCustomParameterEditor(new CustomLabel(param, deltaColour), 120, 95);
    param = getProcessor()->getParameter("delta_high");
    addCustomParameterEditor(new CustomLabel(param, deltaColour), 160, 95);
    param = getProcessor()->getParameter("delta_gain");
    addCustomParameterEditor(new CustomLabel(param, deltaColour), 200, 95);

}
