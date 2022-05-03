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

/**
Editor (in signal chain) contains:
- Input channel selector (filtered output will appear on this channel as well)
- Rolling window duration (ms)
- Low-cut and High-cut frequencies for 3 frequency bands of interest
- Gains for each frequency band
*/

class MultiBandIntegratorEditor
	: public GenericEditor
	, public ComboBox::Listener
	, public Label::Listener
{
public:
    
    /** Constructor */
	MultiBandIntegratorEditor(GenericProcessor* parentNode);
    
    /** Destructor */
	~MultiBandIntegratorEditor();
    
    /** ComboBox::Listener class */
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    /** Label::Listener class */
	void labelTextChanged(Label* labelThatHasChanged) override;

    /** Called when upstream processors update their settings*/
	void updateSettings() override;

    /** Save custom editor parameters */
	void saveCustomParametersToXml(XmlElement* xml) override;
    
    /** Load custom editor parameters*/
	void loadCustomParametersFromXml(XmlElement* xml) override;

private:
	typedef juce::Rectangle<int> Rectangle;

	// Basic UI element creation methods. Always register "this" (the editor) as the listener,
	// but may specify a different Component in which to actually display the element.
	Label* createEditable(const String& name, const String& initialValue,
		const String& tooltip, Rectangle bounds);
	Label* createLabel(const String& name, const String& text, Rectangle bounds);

	// Utilities for parsing entered values
	static bool updateIntLabel(Label* label, int min, int max, int defaultValue, int* out);
	static bool updateFloatLabel(Label* label, float min, float max, float defaultValue, float* out);

	ScopedPointer<Label> inputLabel;
	ScopedPointer<ComboBox> inputBox;

	ScopedPointer<Label> rollLabel1;
	ScopedPointer<Label> rollLabel2;
	ScopedPointer<Label> rollEdit;
	
	// frequency bands
	ScopedPointer<Label> freqLabel;
	ScopedPointer<Label> freqLabelSub;
	ScopedPointer<Label> freqLabelSub2;
	
	ScopedPointer<Label> alphaLowEdit;
	ScopedPointer<Label> alphaHighEdit;
	ScopedPointer<Label> betaLowEdit;
	ScopedPointer<Label> betaHighEdit;
	ScopedPointer<Label> deltaLowEdit;
	ScopedPointer<Label> deltaHighEdit;

	// frequency band gains
	ScopedPointer<Label> gainLabel;

	ScopedPointer<Label> alphaLabel;
	ScopedPointer<Label> alphaEditable;
	ScopedPointer<Label> betaLabel;
	ScopedPointer<Label> betaEditable;
	ScopedPointer<Label> deltaLabel;
	ScopedPointer<Label> deltaEditable;
};



#endif 
