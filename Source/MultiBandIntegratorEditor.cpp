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

MultiBandIntegratorEditor::MultiBandIntegratorEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors)
    : GenericEditor(parentNode, useDefaultParameterEditors)
{
	desiredWidth = 250;

    MultiBandIntegrator* processor = static_cast<MultiBandIntegrator*>(parentNode);

    const int TEXT_HT = 18;
	
	/* ---------------- Frequency ranges and gains --------------- */

    int xPos = 12;
    int yPos = 36;

    inputLabel = createLabel("InputChanL", "In:", Rectangle(xPos, yPos, 30, TEXT_HT));
    addAndMakeVisible(inputLabel);

	rollLabel1 = createLabel("rollL1", "Window", Rectangle(xPos, yPos += 30, 70, TEXT_HT));
	addAndMakeVisible(rollLabel1);

	rollEdit = createEditable("rollE", String(processor->rollDur), "",
		Rectangle(xPos, yPos += 20, 40, TEXT_HT));
	addAndMakeVisible(rollEdit);

	rollLabel2 = createLabel("rollL2", "ms", Rectangle(xPos +=42, yPos, 30, TEXT_HT));
	addAndMakeVisible(rollLabel2);


	xPos = 12;
	yPos = 36;

    inputBox = new ComboBox("Input channel");
    inputBox->setTooltip("Continuous channel to analyze");
    inputBox->setBounds(xPos += 33, yPos, 40, TEXT_HT);
    inputBox->addListener(this);
    addAndMakeVisible(inputBox);

	/* ---------------- Right Panel Frequency ranges and gains --------------- */

	//frequency band low cutoffs
	int xPosR = 90;
	int yPosR = 25;

	freqLabel = createLabel("freqL", "Frequency bands", Rectangle(xPosR, yPosR, 150, TEXT_HT));
	addAndMakeVisible(freqLabel);

	freqLabelSub = createLabel("freqLS", "Low", Rectangle(xPosR, yPosR += 20, 50, TEXT_HT));
	addAndMakeVisible(freqLabelSub);

	alphaLowEdit = createEditable("alphaLowE", String(processor->alphaLow), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(alphaLowEdit);

	betaLowEdit = createEditable("betaLowE", String(processor->betaLow), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(betaLowEdit);

	deltaLowEdit = createEditable("deltaLowE", String(processor->deltaLow), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(deltaLowEdit);

	//frequency band high cutoffs

	xPosR = 140;
	yPosR = 45;

	freqLabelSub2 = createLabel("freqLS2", "High", Rectangle(xPosR, yPosR, 50, TEXT_HT));
	addAndMakeVisible(freqLabelSub2);
	
	alphaHighEdit = createEditable("alphaHighE", String(processor->alphaHigh), "",
		Rectangle(xPosR, yPosR +=20, 40, TEXT_HT));
	addAndMakeVisible(alphaHighEdit);

	betaHighEdit = createEditable("betaHighE", String(processor->betaHigh), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(betaHighEdit);

	deltaHighEdit = createEditable("deltaHighE", String(processor->deltaHigh), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(deltaHighEdit);

	//frequency band gains

	xPosR = 190; 
	yPosR = 45; 
	
	gainLabel = createLabel("gainL", "Gain", Rectangle(xPosR, yPosR, 50, TEXT_HT));
	addAndMakeVisible(gainLabel);

	//alphaLabel = createLabel("alphaL", "6-9Hz", Rectangle(xPosR, yPosR += 20, 100, TEXT_HT));
	//addAndMakeVisible(alphaLabel);

	alphaEditable = createEditable("alphaE", String(processor->alphaGain), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT ));
	addAndMakeVisible(alphaEditable);

	//betaLabel = createLabel("betaL", "13-18Hz", Rectangle(xPosR, yPosR += 20, 100, TEXT_HT));
	//addAndMakeVisible(betaLabel);

	betaEditable = createEditable("betaE", String(processor->betaGain), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(betaEditable);

	//deltaLabel = createLabel("deltaL", "1-4Hz", Rectangle(xPosR, yPosR += 20, 100, TEXT_HT));
	//addAndMakeVisible(deltaLabel);

	deltaEditable = createEditable("deltaE", String(processor->deltaGain), "",
		Rectangle(xPosR, yPosR += 20, 40, TEXT_HT));
	addAndMakeVisible(deltaEditable);


}

MultiBandIntegratorEditor::~MultiBandIntegratorEditor() {}

void MultiBandIntegratorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == inputBox)
        getProcessor()->setParameter(pInputChan, static_cast<float>(inputBox->getSelectedId() - 1));

}

void MultiBandIntegratorEditor::labelTextChanged(Label* labelThatHasChanged)
{
	MultiBandIntegrator* processor = static_cast<MultiBandIntegrator*>(getProcessor());
	
	if (labelThatHasChanged == alphaLowEdit)
	{
		int newVal;
		bool success = updateIntLabel(labelThatHasChanged, 0, INT_MAX, processor->alphaLow, &newVal);

		if (success)
			processor->setParameter(pAlphaLow, newVal);
	}
	else if (labelThatHasChanged == rollEdit)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, 0, FLT_MAX, processor->rollDur, &newVal);

		if (success)
			processor->setParameter(pRollDur, newVal);
	}
	else if (labelThatHasChanged == alphaHighEdit)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, 0, FLT_MAX, processor->alphaHigh, &newVal);

		if (success)
			processor->setParameter(pAlphaHigh, newVal);
	}
	else if (labelThatHasChanged == alphaEditable)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, -FLT_MAX, FLT_MAX, processor->alphaGain, &newVal);

		if (success)
			processor->setParameter(pAlphaGain, newVal);
	}
	else if (labelThatHasChanged == betaLowEdit)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, 0, FLT_MAX, processor->betaLow, &newVal);

		if (success)
			processor->setParameter(pBetaLow, newVal);
	}
	else if (labelThatHasChanged == betaHighEdit)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, 0, FLT_MAX, processor->betaHigh, &newVal);

		if (success)
			processor->setParameter(pBetaHigh, newVal);
	}
	else if (labelThatHasChanged == betaEditable)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, -FLT_MAX, FLT_MAX, processor->betaGain, &newVal);

		if (success)
			processor->setParameter(pBetaGain, newVal);
	}
	else if (labelThatHasChanged == deltaLowEdit)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, 0, FLT_MAX, processor->deltaLow, &newVal);

		if (success)
			processor->setParameter(pDeltaLow, newVal);
	}
	else if (labelThatHasChanged == deltaHighEdit)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, 0, FLT_MAX, processor->deltaHigh, &newVal);

		if (success)
			processor->setParameter(pDeltaHigh, newVal);
	}
	else if (labelThatHasChanged == deltaEditable)
	{
		float newVal;
		bool success = updateFloatLabel(labelThatHasChanged, -FLT_MAX, FLT_MAX, processor->deltaGain, &newVal);

		if (success)
			processor->setParameter(pDeltaGain, newVal);
	}

}



void MultiBandIntegratorEditor::updateSettings()
{
    MultiBandIntegrator* processor = static_cast<MultiBandIntegrator*>(getProcessor());

    // update input combo box
    int numInputs = processor->settings.numInputs;
    int numBoxItems = inputBox->getNumItems();
    if (numInputs != numBoxItems)
    {
        int currId = inputBox->getSelectedId();
        inputBox->clear(dontSendNotification);
        for (int chan = 1; chan <= numInputs; chan++)
            // using 1-based ids since 0 is reserved for "nothing selected"
            inputBox->addItem(String(chan), chan);
        if (numInputs > 0 && (currId < 1 || currId > numInputs))
            inputBox->setSelectedId(1, sendNotificationAsync);
        else
            inputBox->setSelectedId(currId, dontSendNotification);
    }
    
}

void MultiBandIntegratorEditor::startAcquisition()
{
    //inputBox->setEnabled(false);
}

void MultiBandIntegratorEditor::stopAcquisition()
{
    //inputBox->setEnabled(true);
}


void MultiBandIntegratorEditor::saveCustomParameters(XmlElement* xml)
{
    xml->setAttribute("Type", "MultiBandIntegratorEditor");

    MultiBandIntegrator* processor = static_cast<MultiBandIntegrator*>(getProcessor());
    XmlElement* paramValues = xml->createNewChildElement("VALUES");

    // selected channel
    paramValues->setAttribute("inputChanId", inputBox->getSelectedId());

	// window size
	paramValues->setAttribute("windowSize", rollEdit->getText());

	//frequency bands
	paramValues->setAttribute("alphaLow", alphaLowEdit->getText());
	paramValues->setAttribute("alphaHigh", alphaHighEdit->getText());
	paramValues->setAttribute("betaLow", betaLowEdit->getText());
	paramValues->setAttribute("betaHigh", betaHighEdit->getText());
	paramValues->setAttribute("deltaLow", deltaLowEdit->getText());
	paramValues->setAttribute("deltaHigh", deltaHighEdit->getText());

	// frequency gain
	paramValues->setAttribute("alphaGain", alphaEditable->getText());
	paramValues->setAttribute("betaGain", betaEditable->getText());
	paramValues->setAttribute("deltaGain", deltaEditable->getText());
}

void MultiBandIntegratorEditor::loadCustomParameters(XmlElement* xml)
{
    MultiBandIntegrator* processor = static_cast<MultiBandIntegrator*>(getProcessor());

    forEachXmlChildElementWithTagName(*xml, xmlNode, "VALUES")
    {
        // channels
        inputBox->setSelectedId(xmlNode->getIntAttribute("inputChanId", inputBox->getSelectedId()), sendNotificationAsync);

		// window size
		rollEdit->setText(xmlNode->getStringAttribute("windowSize", rollEdit->getText()), sendNotificationAsync);
       
		// frequency bands
		alphaLowEdit->setText(xmlNode->getStringAttribute("alphaLow", alphaLowEdit->getText()), sendNotificationAsync);
		alphaHighEdit->setText(xmlNode->getStringAttribute("alphaHigh", alphaHighEdit->getText()), sendNotificationAsync);
		betaLowEdit->setText(xmlNode->getStringAttribute("betaLow", betaLowEdit->getText()), sendNotificationAsync);
		betaHighEdit->setText(xmlNode->getStringAttribute("betaHigh", betaHighEdit->getText()), sendNotificationAsync);
		deltaLowEdit->setText(xmlNode->getStringAttribute("deltaLow", deltaLowEdit->getText()), sendNotificationAsync);
		deltaHighEdit->setText(xmlNode->getStringAttribute("deltaHigh", deltaHighEdit->getText()), sendNotificationAsync);

		// frequency gains
		alphaEditable->setText(xmlNode->getStringAttribute("alphaGain", alphaEditable->getText()), sendNotificationAsync);
		betaEditable->setText(xmlNode->getStringAttribute("betaGain", betaEditable->getText()), sendNotificationAsync);
		deltaEditable->setText(xmlNode->getStringAttribute("deltaGain", deltaEditable->getText()), sendNotificationAsync);
      
        }  
}

/**************** private ******************/

Label* MultiBandIntegratorEditor::createEditable(const String& name, const String& initialValue,
    const String& tooltip, Rectangle bounds)
{
    Label* editable = new Label(name, initialValue);
    editable->setEditable(true);
    editable->addListener(this);
    editable->setBounds(bounds);
    editable->setColour(Label::backgroundColourId, Colours::grey);
    editable->setColour(Label::textColourId, Colours::white);
    if (tooltip.length() > 0)
        editable->setTooltip(tooltip);
    return editable;
}

Label* MultiBandIntegratorEditor::createLabel(const String& name, const String& text, Rectangle bounds)
{
    Label* label = new Label(name, text);
    label->setBounds(bounds);
    label->setFont(Font("Small Text", 12, Font::plain));
    label->setColour(Label::textColourId, Colours::darkgrey);
    return label;
}

/* Attempts to parse the current text of a label as an int between min and max inclusive.
*  If successful, sets "*out" and the label text to this value and and returns true.
*  Otherwise, sets the label text to defaultValue and returns false.
*/
bool MultiBandIntegratorEditor::updateIntLabel(Label* label, int min, int max, int defaultValue, int* out)
{
    const String& in = label->getText();
    int parsedInt;
    try
    {
        parsedInt = std::stoi(in.toRawUTF8());
    }
    catch (const std::logic_error&)
    {
        label->setText(String(defaultValue), dontSendNotification);
        return false;
    }

    if (parsedInt < min)
        *out = min;
    else if (parsedInt > max)
        *out = max;
    else
        *out = parsedInt;

    label->setText(String(*out), dontSendNotification);
    return true;
}

// Like updateIntLabel, but for floats
bool MultiBandIntegratorEditor::updateFloatLabel(Label* label, float min, float max, float defaultValue, float* out)
{
    const String& in = label->getText();
    float parsedFloat;
    try
    {
        parsedFloat = std::stof(in.toRawUTF8());
    }
    catch (const std::logic_error&)
    {
        label->setText(String(defaultValue), dontSendNotification);
        return false;
    }

    if (parsedFloat < min)
        *out = min;
    else if (parsedFloat > max)
        *out = max;
    else
        *out = parsedFloat;

    label->setText(String(*out), dontSendNotification);
    return true;
}

