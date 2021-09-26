/*
  ==============================================================================
	MainComponent.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../audio/Audio.h"
#include "./fileui/FilePlayerGui.h"
#include "./fileui/FileManagerComponent.h"
#include "./trackerui/TrackerComponent.h"

/**  This class is a component used to control the GUI. */
class MainComponent		:	public Component,
							public MenuBarModel
{
public:
	/** Constructor. 
		@param reference to an Audio object */
	MainComponent(Audio& a);

	/** Destructor */
	~MainComponent();

	//Component
	void resized() override;
	void paint(Graphics&) override;

	//MenuBarModel
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

	enum Menus
	{
		FileMenu = 0,

		NumMenus
	};

	enum FileMenuItems
	{
		AudioPrefs = 1,

		NumFileItems
	};

private:
	Audio& audio;

	TabbedComponent tabs;
	TrackerComponent trackerComponent;
	Viewport fileManagerViewport;
	FileManagerComponent fileManagerComponent;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
