/*
  ==============================================================================
	TrackerCellGui.cpp
  ==============================================================================
*/

#include "TrackerCellGui.h"

TrackerCellGui::TrackerCellGui() : eventInfo({"1", "-1", "1"})
{
	noteTextEditor.setJustification(Justification::centred);
	noteTextEditor.setTextToShowWhenEmpty("---", getLookAndFeel().findColour(juce::TextEditor::textColourId));
	noteTextEditor.setInputRestrictions(3, "012345678#ABCDEFGabcdefg");
	noteTextEditor.addListener(this);
	addAndMakeVisible(noteTextEditor);

	sampleTextEditor.setJustification(Justification::centred);
	sampleTextEditor.setTextToShowWhenEmpty("--", getLookAndFeel().findColour(juce::TextEditor::textColourId));
	sampleTextEditor.setInputRestrictions(2, "0123456789");
	sampleTextEditor.addListener(this);
	addAndMakeVisible(sampleTextEditor);

	gainTextEditor.setJustification(Justification::centred);
	gainTextEditor.setTextToShowWhenEmpty("---", getLookAndFeel().findColour(juce::TextEditor::textColourId));
	gainTextEditor.setInputRestrictions(3, "0.123456789");
	gainTextEditor.addListener(this);
	addAndMakeVisible(gainTextEditor);
}

TrackerCellGui::~TrackerCellGui()
{

}

std::array<String, 3> TrackerCellGui::getEventInfo()
{
	return eventInfo;
}

int TrackerCellGui::getMidiNoteNumber(String noteOctave)
{
	//gets the position of the first number in the string
	int numberPosition = noteOctave.indexOfAnyOf("0123456789");

	//gets the first note in the string and ensures it is uppercase
	//this is to allow finding the note in the noteNames StringArray later
	String note = noteOctave.substring(0, numberPosition).toUpperCase();
	//gets the octave number of the note
	int octave = noteOctave.substring(numberPosition).getIntValue() + 1;

	//returns the midi note number by finding the corresponding position
	//of the note in the StringArray (C is 0, B is 11) and adding it to the octave
	//value multiplied by 12 (to account for each note in each octave)
	return noteNames.indexOf(note) + (octave * 12);
}

bool TrackerCellGui::isMidiNoteValid(String noteOctave)
{
	//gets the position of the first note in the string
	int notePosition = noteOctave.indexOfAnyOf("abcdefgABCDEFG");
	//gets the position of the last note in the string
	int lastNotePosition = noteOctave.lastIndexOfAnyOf("abcdefgABCDEFG");
	//gets the position of the first number in the string
	int numberPosition = noteOctave.indexOfAnyOf("0123456789");
	//gets the midi note number of the string
	int noteNumber = getMidiNoteNumber(noteOctave);

	//if the first note in the string is at the first position of the string
	if (notePosition == 0)
	{
		//and there is only one note in the string
		if (lastNotePosition <= 0)
		{
			//and the octave number is in either the second or third position, not both (i.e. its a single digit)
			if ((numberPosition == 1) ^ (numberPosition == 2))
			{
				//and the midi note number is within the bounds of the midi spec
				if (noteNumber >= 21 && noteNumber <= 108)
				{
					//then the note is valid!
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

//TextEditor listener
void TrackerCellGui::textEditorTextChanged(TextEditor& textEditor)
{
	if (&textEditor == &noteTextEditor)
	{
		//if noteTextEditor.getText() is a valid MIDI note 
		if (isMidiNoteValid(textEditor.getText()))
		{
			//return the pitch ratio to be used by a ResamplingAudioSource
			//this assumes all samples are at C4 - hence divide by the frequency of C4
			//if note noteTextEditor.getText() is C4, pitch ratio will be 1, if C5, pitch ratio will be 2, etc.
			pitchRatio = getMidiNoteInHertz(getMidiNoteNumber(textEditor.getText())) / 261.626;
			//set the first element in the array eventInfo to the given pitch ratio
			eventInfo[0] = String(pitchRatio);
		}
		//if noteTextEditor.getText() is not a valid MIDI note 
		else
		{
			//set the first element in the array eventInfo to 1 (i.e. just play at C4)
			eventInfo[0] = String(1);
		}
	}
	if (&textEditor == &sampleTextEditor)
	{
		//if sampleTextEditor.getText() is a valid sample number
		if (textEditor.getText().getIntValue() >= 0 && textEditor.getText().getIntValue() <= 31)
		{
			//set the second element in the array eventInfo to the given sample number
			eventInfo[1] = String(textEditor.getText());
		}
		else
		{
			//set the second element in the array eventInfo to -1:
			//this should be interpretted by other classes as an invalid sample number
			eventInfo[1] = String(-1);
		}
	}
	if (&textEditor == &gainTextEditor)
	{
		//if sampleTextEditor.getText() is a valid gain value
		if (textEditor.getText().getFloatValue() >= 0.f && textEditor.getText().getFloatValue() <= 1.f)
		{
			//set the third element in the array eventInfo to the given gain
			eventInfo[2] = String(textEditor.getText());
		}
		else
		{
			//set the third element in the array eventInfo to 0 - this will cause the sample to play silently
			eventInfo[2] = String(0);
		}
	}
}

//Component
void TrackerCellGui::resized()
{
	auto r = getLocalBounds();
	r.removeFromRight(40);
	noteTextEditor.setBounds(r.removeFromLeft(getWidth() / 3));
	sampleTextEditor.setBounds(r.removeFromLeft(getWidth() / 3));
	gainTextEditor.setBounds(r.removeFromLeft(getWidth() / 3));
}

void TrackerCellGui::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}