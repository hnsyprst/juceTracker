/*
  ==============================================================================
	TrackerCellGui.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/audio/fileaudio/FilePlayer.h"

/** GUI for the FilePlayer class. */

class TrackerCellGui		:	public Component,
								public TextEditor::Listener,
								public MidiMessage
{
public:
	/** Constructor. */
	TrackerCellGui();

	/** Destructor. */
	~TrackerCellGui();

	/** Returns the array of Strings representing the user-inputted event info held by this object.
		@return	std::array of String, size [3]
		@see	textEditorTextChanged */
	std::array<String, 3> getEventInfo();

	/** Returns int MIDI note number for the human-readable note name passed to this method.
		It is advisable to first check that the note name being passed is valid using isMidiNoteValid.
		@param String human-readable note name
		@return	int MIDI note number
		@see isMidiNoteValid */
	int getMidiNoteNumber(String noteOctave);

	/** Returns true if the human-readable note name passed to this method is a valid note.
		@param String human-readable note name
		@return	bool validity of the human-readable note name noteOctave */
	bool isMidiNoteValid(String noteOctave);

	//TextEditor::Listener
	/** Overridden function inherited from TextEditor::Listener. Performs input validity checks, then
		sets the elements of eventInfo to the parameters entered by the user in the Tracker interface.
		@param pointer to the TextEditor that was changed */
	void textEditorTextChanged(TextEditor &textEditor) override;

	//Component
	void resized() override;
	void paint(Graphics&) override;

private:
	TextEditor noteTextEditor;
	TextEditor sampleTextEditor;
	TextEditor gainTextEditor;

	std::array<String, 3> eventInfo;
	StringArray noteNames = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	double pitchRatio;
};
