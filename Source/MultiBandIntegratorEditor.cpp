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

CustomLabel::CustomLabel(Parameter* param) : ParameterEditor(param)
{
    label.addListener(this);
    label.setBounds(0, 0, 50, 15);
    label.setEditable(true);
    addAndMakeVisible(&label);
    setBounds(0, 0, 50, 15);
}

void CustomLabel::labelTextChanged(Label*)
{
    param->setNextValue(label.getText().getFloatValue());
}

void CustomLabel::updateView()
{
    label.setText(param->getValueAsString(), dontSendNotification);
}


MultiBandIntegratorEditor::MultiBandIntegratorEditor(GenericProcessor* parentNode)
    : GenericEditor(parentNode)
{
	desiredWidth = 250;

    addSelectedChannelsParameterEditor("Channel", 15, 38);
    addTextBoxParameterEditor("window_ms", 15, 72);
    
    Parameter* param = getProcessor()->getParameter("alpha_low");
    addCustomParameterEditor(new CustomLabel(param), 110, 45);
    param = getProcessor()->getParameter("alpha_high");
    addCustomParameterEditor(new CustomLabel(param), 110, 70);
    param = getProcessor()->getParameter("alpha_gain");
    addCustomParameterEditor(new CustomLabel(param), 110, 95);
    
    param = getProcessor()->getParameter("beta_low");
    addCustomParameterEditor(new CustomLabel(param), 160, 45);
    param = getProcessor()->getParameter("beta_high");
    addCustomParameterEditor(new CustomLabel(param), 160, 70);
    param = getProcessor()->getParameter("beta_gain");
    addCustomParameterEditor(new CustomLabel(param), 160, 95);
    
    param = getProcessor()->getParameter("delta_low");
    addCustomParameterEditor(new CustomLabel(param), 210, 45);
    param = getProcessor()->getParameter("delta_high");
    addCustomParameterEditor(new CustomLabel(param), 210, 70);
    param = getProcessor()->getParameter("delta_gain");
    addCustomParameterEditor(new CustomLabel(param), 210, 95);

}
