/*
  ==============================================================================
	FileManagerComponent.cpp
  ==============================================================================
*/

#include "FileManagerComponent.h"


//passes each FilePlayerGui its index in the array and the corresponding FilePlayer for it to control
FileManagerComponent::FileManagerComponent(Audio& a)		:	audio(a)
{
	for (int i = 0; i < Audio::NumberOfFilePlayers; i++)
	{
		//pass each FilePlayerGui its index in the array it is stored in within this object
		filePlayerGui[i].setIndex(i);
		//pass each FilePlayerGui the FilePlayer it will control
		filePlayerGui[i].setFilePlayer(audio.getFilePlayer(i));
		addAndMakeVisible(filePlayerGui[i]);
	}
}

FileManagerComponent::~FileManagerComponent()
{

}

void FileManagerComponent::resized()
{
	auto r = getLocalBounds();

	for (int i = 0; i < Audio::NumberOfFilePlayers; i++)
	{
		filePlayerGui[i].setBounds(r.removeFromTop(40));
	}
}
void FileManagerComponent::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}