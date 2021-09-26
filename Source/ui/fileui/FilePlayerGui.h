/*
  ==============================================================================
	FilePlayerGui.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/audio/fileaudio/FilePlayer.h"

/** GUI for the FilePlayer class. */

class FilePlayerGui		:	public Component,
							private Button::Listener,
							private Slider::Listener,
							private FilenameComponentListener
{
public:
	/** Constructor. */
	FilePlayerGui();

	/** Destructor. */
	~FilePlayerGui();
	
	/** Sets the file player that this GUI controls.
		@param	FilePlayer to be controlled by this GUI 
		@see	FilePlayer */
	void setFilePlayer(FilePlayer* fp);

	/** Passes this object its index in the array it has been created in and sets the Label
		component text and background colour of the GUI's child components accordingly.
		@param	int expected to be the index of this object in the array it has been created in */
	void setIndex(int newIndex);

	//Button::Listener
	/** Overridden function inherited from Button::Listener. If the play button has been pressed, flips the
		play state of the FilePlayer object this object controls and provides GUI feedback of this change.
		If the loop button has been pressed, flips the loop state of the FilePlayer object this object controls
		and provides GUI feedback of this change.
		@param pointer to the Button that was clicked */
	void buttonClicked(Button* button) override;

	//Slider::Listener
	/** Overridden function inherited from Slider::Listener. Functionality not currently implemented. Will
		act as a sample offset scrubber allowing playback of audio files to begin at points other than the start of the file.
		@param pointer to the Slider that was changed */
	void sliderValueChanged(Slider* slider) override;

	//FilenameComponent::Listener
	/** Overridden function inherited from FilenameComponent::Listener. Passes the file selected by the user to the FilePlayer
		object this object controls.
		@param pointer to the FilenameComponent that was changed */
	void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

	//Component
	void resized() override;
	void paint(Graphics&) override;

private:
	Label indexLabel		{	"N/A"	};
	TextButton playButton	{	">"		};
	TextButton loopButton	{	"Loop"	};
	std::unique_ptr<FilenameComponent> fileChooser;
	Slider pitchSlider;

	int index;
	Colour colour;

	FilePlayer* filePlayer	{	nullptr	};
};
