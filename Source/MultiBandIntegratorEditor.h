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

#ifndef MULTIBAND_INTEGRATOR_EDITOR_H_INCLUDED
#define MULTIBAND_INTEGRATOR_EDITOR_H_INCLUDED

#include <EditorHeaders.h>
#include <string>
#include <climits>
#include <cfloat>
#include <algorithm>


class CustomLabel
    : public ParameterEditor,
      public Label::Listener
{
public:
    /** Constructor*/
    CustomLabel(Parameter* param);

    /** Destructor */
    ~CustomLabel() { }

    /** Respond to text input*/
    void labelTextChanged(Label*);

    /** Updates the view*/
    void updateView();
    
private:
    
    Label label;
    
};

/**
Editor (in signal chain) contains:
- Input channel selector (filtered output will appear on this channel as well)
- Rolling window duration (ms)
- Low-cut and High-cut frequencies for 3 frequency bands of interest
- Gains for each frequency band
*/

class MultiBandIntegratorEditor
	: public GenericEditor
{
public:
    
    /** Constructor */
	MultiBandIntegratorEditor(GenericProcessor* parentNode);
    
    /** Destructor */
    ~MultiBandIntegratorEditor() { }
};



#endif 
