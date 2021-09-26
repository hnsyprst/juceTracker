/*
  ==============================================================================
	MainComponent.cpp
  ==============================================================================
*/

#include "MainComponent.h"

MainComponent::MainComponent(Audio& a)		:	audio(a),
												fileManagerComponent(a),
												trackerComponent(audio.getFilePlayerArray(), a),
												tabs(TabbedButtonBar::Orientation::TabsAtTop)
{
	fileManagerViewport.setViewedComponent(&fileManagerComponent);
	//hides the redundant horizontal scrollbar
	fileManagerViewport.setScrollBarsShown(true,
											false,
											true,
											false);

	//creates, fills and makes visible the two tabs that will be used to display the two parts of the user interface
	tabs.addTab("Sample Manager", getLookAndFeel().findColour(juce::TabbedComponent::backgroundColourId), &fileManagerViewport, true);
	tabs.addTab("Tracker", getLookAndFeel().findColour(juce::TabbedComponent::backgroundColourId), &trackerComponent, true);
	addAndMakeVisible(tabs);

	setSize(1280, 720);
}

MainComponent::~MainComponent()
{

}

void MainComponent::resized()
{
	auto r = getLocalBounds();
	tabs.setBounds(r);

	//each FilePlayerGui is 40px high - set the height of this component to be 40 * number of FilePlayerGui objects
	//does not matter if this component is larger than the size of the window - it will be scrollable via the Viewport
	fileManagerComponent.setSize(fileManagerViewport.getWidth(), (40 * Audio::NumberOfFilePlayers));
}
void MainComponent::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

StringArray MainComponent::getMenuBarNames()
{
	auto names = { "File" };
	return StringArray(names);
}

PopupMenu MainComponent::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
{
	PopupMenu menu;
	if (topLevelMenuIndex == 0)
		menu.addItem(AudioPrefs, "Audio Prefrences", true, false);
	return menu;
}

void MainComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	if (topLevelMenuIndex == FileMenu)
	{
		if (menuItemID == AudioPrefs)
		{
			DialogWindow::LaunchOptions la;
			la.dialogTitle = "Audio Settings";
			OptionalScopedPointer<Component> osp(std::make_unique<AudioDeviceSelectorComponent>
				(audio.getAudioDeviceManager(),
					1, 2, 2, 2,
					true, true, true, false));
			osp->setSize(450, 350);
			la.content = std::move(osp);
			la.componentToCentreAround = this;
			la.launchAsync();
		}
	}
}
