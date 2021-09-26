/*
  ==============================================================================
	FilePlayerGui.cpp
  ==============================================================================
*/

#include "FilePlayerGui.h"

FilePlayerGui::FilePlayerGui()		:	index(0),
										colour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId))
{
	indexLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(indexLabel);

	playButton.addListener(this);
	addAndMakeVisible(playButton);
	loopButton.addListener(this);
	addAndMakeVisible(loopButton);

	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	fileChooser = std::make_unique<FilenameComponent>("audiofile",
														File(),
														true, false, false,
														formatManager.getWildcardForAllFormats(),
														String(),
														"(select an audio file)");
	fileChooser->addListener(this);
	addAndMakeVisible(fileChooser.get());

	pitchSlider.addListener(this);
	pitchSlider.setValue(1.f);
	pitchSlider.setRange(0.01, 5.f);
	addAndMakeVisible(pitchSlider);
}

FilePlayerGui::~FilePlayerGui()
{

}

void FilePlayerGui::setFilePlayer(FilePlayer* fp)
{
	filePlayer = fp;
}

void FilePlayerGui::setIndex(int newIndex)
{
	index = newIndex;
	indexLabel.setText(String(index), dontSendNotification);

	//if the index is not an even number, set a different colour to normal
	//this helps to make the interface more readable by more clearly seperating
	//the different FilePlayerGui objects
	if ((index % 2) != 0)
	{
		colour = juce::Colour(69, 86, 94);
	}
}

//Component
void FilePlayerGui::resized()
{
	auto r = getLocalBounds();
	indexLabel.setBounds(r.removeFromLeft(getHeight()));

	auto row = r.removeFromTop(getHeight() / 2);
	playButton.setBounds(row.removeFromLeft(getHeight()));
	fileChooser->setBounds(row);

	auto row2 = r.removeFromTop(getHeight() / 2);
	loopButton.setBounds(row2.removeFromLeft(getHeight()));
	pitchSlider.setBounds(row2);
}

void FilePlayerGui::paint(Graphics& g)
{
	g.fillAll(colour);
	g.setColour(colour);
	loopButton.setColour(TextButton::buttonColourId, colour);
	loopButton.setColour(TextButton::buttonOnColourId, Colours::red);
}

//Button listener
void FilePlayerGui::buttonClicked(Button* button)
{
	//only perform operations if a filePlayer has actually been passed to this object
	if (filePlayer != nullptr)
	{
		if (button == &playButton)
		{
			filePlayer->setPlaying(!filePlayer->isPlaying());
		}
		else if (button == &loopButton)
		{
			loopButton.setToggleState(!filePlayer->isLooping(), dontSendNotification);
			filePlayer->setLooping(!filePlayer->isLooping());
		}
	}
}

//Slider listener
void FilePlayerGui::sliderValueChanged(Slider* slider)
{
	/* This will be changed to a sample offset scrubber later

	if (filePlayer != nullptr && slider == &pitchSlider)
	{
		filePlayer->setPlaybackRate(slider->getValue());
	}*/
}

//FilenameComponent listener
void FilePlayerGui::filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged)
{
	//if the fileChooser has been modified, set audioFile to the chosen file
	if (fileComponentThatHasChanged == fileChooser.get())
	{
		File audioFile(fileChooser->getCurrentFile().getFullPathName());

		//only set the file if a filePlayer has actually been passed to this object
		//and if the chosen file actually exists
		if (filePlayer != nullptr && audioFile.existsAsFile())
		{
			filePlayer->loadFile(audioFile);
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon,
										"File error",
										"Couldn't open file!");
		}
	}
}