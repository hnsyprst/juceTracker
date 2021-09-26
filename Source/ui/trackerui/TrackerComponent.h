/*
  ==============================================================================
	TrackerComponent.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/audio/Audio.h"
#include "../Source/audio/Counter.h"
#include "TrackerCellGui.h"

/** This class is a component used to contain, manage and display an array of TrackerCellGui objects. */

class TrackerComponent		:	public Component,
								public Button::Listener,
								public TextEditor::Listener,
								public Counter::Listener
{
public:
	/** Constructor.
		@param std::array of FilePlayer objects of size given by Audio::NumberOfFilePlayers
		@param reference to an Audio object */
	TrackerComponent(std::array<FilePlayer, Audio::NumberOfFilePlayers>& fpa, Audio& a);

	/** Destructor. */
	~TrackerComponent();

	/** Holds number of rows of TrackerCellGui objects to be shown in tracker. */
	enum
	{
		NumberOfRowsPerPattern = 64
	};

	/** Returns true if the bpm value passed to this method is a valid bpm.
		@param String bpm value
		@return	bool validity of the bpm String bpmString */
	bool isBpmValid(String bpmString);

	//Button::Listener
	/** Overridden function inherited from Button::Listener. Flips the
		play state of the Counter and Audio objects and provides GUI feedback of this change.
		@param pointer to the Button that was clicked */
	void buttonClicked(Button* button) override;

	//TextEditor::Listener
	/** Overridden function inherited from TextEditor::Listener. Performs input validity checks, then
		sets the bpm variable in this object and in the Audio object to value entered by the user in the TextEditor.
		@param pointer to the TextEditor that was changed */
	void textEditorTextChanged(TextEditor& textEditor) override;

	//Counter::Listener
	/** Overridden function inherited from Counter::Listener. Passes this object the current counter value when
		the count has changed.
		@param current counter value */
	void counterChanged(int counterValue) override;

	//Comoponent
	void resized() override;
	void paint(Graphics&) override;

private:
	std::array<FilePlayer, Audio::NumberOfFilePlayers>& filePlayerArray;
	std::array<std::array<TrackerCellGui, 4>, 64> trackerCellGuiArray;
	Audio& audio;

	Viewport trackerViewport;
	Component trackerGridComponent;

	std::array<Label, NumberOfRowsPerPattern> trackerRowLabelArray;
	std::array<std::array<Rectangle<int>, 4>, TrackerComponent::NumberOfRowsPerPattern> seperatorArray;

	TextButton playButton		{	">"		};
	TextEditor bpmEditor;
	int bpm;

	Label channelNumberLabel1;
	Label channelNumberLabel2;
	Label channelNumberLabel3;
	Label channelNumberLabel4;
	Label trackerRowLabelCorner;

	Counter counter;
};
