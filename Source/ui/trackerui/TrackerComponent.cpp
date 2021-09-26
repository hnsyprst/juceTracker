/*
  ==============================================================================
	TrackerComponent.cpp
  ==============================================================================
*/

//IMPLEMENT NULLPTR CHECKS

#include "TrackerComponent.h"

TrackerComponent::TrackerComponent(std::array<FilePlayer, Audio::NumberOfFilePlayers>& fpa, Audio& a)
																											:	filePlayerArray(fpa),
																												audio(a),
																												bpm(130),
																												counter(TrackerComponent::NumberOfRowsPerPattern)
{
	audio.setTrackerCellGuiArray(trackerCellGuiArray);
	counter.setListener(this);

	//bpmEditor formatting and setup
	bpmEditor.addListener(this);
	addAndMakeVisible(bpmEditor);
	bpmEditor.setJustification(Justification::centred);
	bpmEditor.setTextToShowWhenEmpty("130", getLookAndFeel().findColour(juce::TextEditor::textColourId));
	bpmEditor.setInputRestrictions(3, "0123456789");

	//playButton setup
	playButton.addListener(this);
	addAndMakeVisible(playButton);

	//channelNumberLabels formatting and setup
	channelNumberLabel1.setJustificationType(Justification::centred);
	channelNumberLabel1.setText("Channel 1", dontSendNotification);
	addAndMakeVisible(channelNumberLabel1);

	channelNumberLabel2.setJustificationType(Justification::centred);
	channelNumberLabel2.setText("Channel 2", dontSendNotification);
	addAndMakeVisible(channelNumberLabel2);

	channelNumberLabel3.setJustificationType(Justification::centred);
	channelNumberLabel3.setText("Channel 3", dontSendNotification);
	addAndMakeVisible(channelNumberLabel3);

	channelNumberLabel4.setJustificationType(Justification::centred);
	channelNumberLabel4.setText("Channel 4", dontSendNotification);
	addAndMakeVisible(channelNumberLabel4);

	//creates an extra, empty box in top left corner to improve formatting
	addAndMakeVisible(trackerRowLabelCorner);

	//trackerRowLabels and trackerCellGuis formatting and setup in a grid
	for (int row = 0; row < trackerCellGuiArray.size(); row++)
	{
		for (int col = 0; col < trackerCellGuiArray[0].size(); col++)
		{
			trackerRowLabelArray[row].setJustificationType(Justification::centred);
			if (row % 4 == 0)
			{
				//trackerRowLabelArray[row].getFont().setBold(true);
				trackerRowLabelArray[row].setFont(juce::Font(16.0f, juce::Font::bold));
			}
			trackerGridComponent.addAndMakeVisible(trackerRowLabelArray[row]);
			trackerRowLabelArray[row].setText(String(row), dontSendNotification);
			trackerGridComponent.addAndMakeVisible(trackerCellGuiArray[row][col]);
		}
	}

	trackerGridComponent.setSize(getParentWidth(), (40 * TrackerComponent::NumberOfRowsPerPattern));

	trackerViewport.setViewedComponent(&trackerGridComponent);
	//hides the redundant horizontal scrollbar
	trackerViewport.setScrollBarsShown(true,
										false,
										true,
										false);
	addAndMakeVisible(trackerViewport);

	setSize(1280, 720);
}

TrackerComponent::~TrackerComponent()
{

}

void TrackerComponent::resized()
{
	//sets play button and bpm box position
	auto r = getLocalBounds();
	auto firstRow = r.removeFromTop(40);
	bpmEditor.setBounds(firstRow.removeFromRight(40));
	playButton.setBounds(firstRow);

	//sets the channel number labels poisitions
	auto secondRow = r.removeFromTop(40);
	trackerRowLabelCorner.setBounds(secondRow.removeFromLeft(40));
	channelNumberLabel1.setBounds(secondRow.removeFromLeft(getWidth() / 4));
	channelNumberLabel2.setBounds(secondRow.removeFromLeft(getWidth() / 4));
	channelNumberLabel3.setBounds(secondRow.removeFromLeft(getWidth() / 4));
	channelNumberLabel4.setBounds(secondRow);

	trackerViewport.setBounds(r);
	auto trackerBounds = trackerGridComponent.getLocalBounds();

	//sets trackerRowLabels and trackerCellGuis positions in a grid
	for (int row = 0; row < trackerCellGuiArray.size(); row++)
	{
		auto rowBounds = trackerBounds.removeFromTop(40);
		trackerRowLabelArray[row].setBounds(rowBounds.removeFromLeft(40));
		for (int col = 0; col < trackerCellGuiArray[0].size(); col++)
		{
			trackerCellGuiArray[row][col].setBounds(rowBounds.removeFromLeft(getWidth() / 4));
		}
	}
}

void TrackerComponent::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

bool TrackerComponent::isBpmValid(String bpmString)
{
	// if the bpm is 0, it is invalid, otherwise it is valid
	if (bpmString.getIntValue() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Button listener
void TrackerComponent::buttonClicked(Button* button)
{
	//flips the run state of the counter and changes playButton text / trackerRowLabelArray colour accordingly
	if (button == &playButton)
	{
		if (counter.isRunning())
		{
			button->setButtonText(">");
			counter.setRunState(false, bpm);
			audio.setRunState(false);
			for (int row = 0; row < trackerRowLabelArray.size(); row++)
			{
				trackerRowLabelArray[row].setColour(juce::Label::textColourId, juce::Colours::white);
			}
		}
		else
		{
			button->setButtonText("||");
			counter.setRunState(true, bpm);
			audio.setRunState(true);
		}
	}
}

//TextEditor listener
void TrackerComponent::textEditorTextChanged(TextEditor& textEditor)
{
	//sets the bpm variable if it has been changed and the bpm is valid
	if (&textEditor == &bpmEditor)
	{
		if (isBpmValid(textEditor.getText()))
		{
			bpm = textEditor.getText().getIntValue();
			audio.setBpm(bpm);
		}
	}
}

//Counter listener
void TrackerComponent::counterChanged(int counterValue)
{
	//when the counter object broadcasts a change in count, reflect this in the GUI

	//callAsync posts this lambda function from the Counter thread to the message queue to
	//be completed when possible
	MessageManager::callAsync([=]()
	{
		//changes the colour of the currently playing row's label to red
		//and the previously playing row's label back to white
		if (counterValue == 0)
		{
			trackerRowLabelArray[counterValue].setColour(juce::Label::textColourId, juce::Colours::red);
			trackerRowLabelArray.back().setColour(juce::Label::textColourId, juce::Colours::white);
		}
		else
		{
			trackerRowLabelArray[counterValue].setColour(juce::Label::textColourId, juce::Colours::red);
			trackerRowLabelArray[counterValue - 1].setColour(juce::Label::textColourId, juce::Colours::white);
		}
	});
}