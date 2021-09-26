/*
  ==============================================================================
	FileManagerComponent.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Source/audio/Audio.h"
#include "FilePlayerGui.h"

/** This class is a component used to contain, manage and display an array of FilePlayerGui objects. */

class FileManagerComponent		:	public Component
{
public:
	/** Constructor.
	@param	reference to an Audio object, expected to be the main Audio object of the application */
	FileManagerComponent(Audio& a);

	/** Destructor. */
	~FileManagerComponent();

	//Component
	void resized() override;
	void paint(Graphics&) override;

private:
	Audio& audio;
	std::array<FilePlayerGui, Audio::NumberOfFilePlayers> filePlayerGui;
};
