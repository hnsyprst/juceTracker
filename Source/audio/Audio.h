/*
  ==============================================================================
	Audio.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>
#include "fileaudio/FilePlayer.h"
#include "../ui/trackerui/TrackerCellGui.h"

/** Class containing all audio processes. */

class Audio		:	public AudioIODeviceCallback
{
public:
	/** Constructor. */
	Audio();

	/** Destructor. */
	~Audio();

	/** Holds number of samples useable in tracker. */
	enum
	{
		NumberOfFilePlayers = 32
	};

	/** Returns a reference to the FilePlayer specified by index. Be careful to check that
		a FilePlayer has actually been returned! nullptr will be returned if index is out of range.
		@param	int index in the range of NumberOfFilePlayers
		@return	reference to the FilePlayer object at the given index
		@see	NumberOfFilePlayers */
	FilePlayer* getFilePlayer(int index);

	/** Returns a reference to the FilePlayerArray held by this object.
		@return	reference to the FilePlayerArray held by this object
		@see	NumberOfFilePlayers */
	std::array<FilePlayer, NumberOfFilePlayers>& getFilePlayerArray();

	/** Returns the audio device manager, don't keep a copy of it!
		@return reference to the AudioDeviceManager created by this object */
	AudioDeviceManager& getAudioDeviceManager() { return audioDeviceManager; }

	/** Sets the rate at which the events held in TrackerCellGuis will be read in beats per minute.
		Internally this method calculates the rate in milliseconds from the given bpm value and 
		assigns it to tempo.
		@param	int rate at which events held in TrackerCellGuis will be read in beats per minute */
	void setBpm(int bpm);

	/** Sets the running state of the tracker - true will start playback,
		false will end playback.
		@param	bool new running state */
	void setRunState(bool rs);

	/** Passes this class a reference to a two dimensional array of TrackerCellGuis for
		(*trackerCellGuiArray) to point to.
		@param	reference to a two dimensional array of TrackerCellGuis of size [4][64] */
	void setTrackerCellGuiArray(std::array<std::array<TrackerCellGui, 4>, 64>& trackerCellGuiArray);

	//AudioIODeviceCallback
	/** Overridden function inherited from AudioIODeviceCallback. Processes a block of audio data.
		Counts samples and rows to trigger musical events at 16th note divisions of the user-specified BPM.
		@param	float** pointer to a 2D array of type float containing the incoming audio data for each audio channel
		@param	int number of channels of incoming audio data
		@param	float** pointer to a 2D array of type float to fill with the outgoing audio data for each audio channel
		@param	int number of channels of outgoing audio data
		@param	int number of samples in each channel of the input and output arrays (dependent on the audio device's buffer size) */
	void audioDeviceIOCallback(const float** inputChannelData,
								int numInputChannels,
								float** outputChannelData,
								int numOutputChannels,
								int numSamples) override;
	/** Overridden function inherited from AudioIODeviceCallback. Called when the audio device is about to start calling back.
		@param pointer to an AudioIODevice object to get incoming audio data from and push outgoing audio data to */
	void audioDeviceAboutToStart(AudioIODevice* device) override;
	/** Overridden function inherited from AudioIODeviceCallback. Called when the audio device has stopped. */
	void audioDeviceStopped() override;

private:
	double sampleRate;
	int sampleCounter;
	int rowCounter;
	int tempo;
	bool runState;
	AudioDeviceManager audioDeviceManager;
	AudioSourcePlayer audioSourcePlayer;
	MixerAudioSource mixerAudioSource;
	std::array<TrackerCellGui,4> (*trackerCellGuiArray);
	std::array<FilePlayer, NumberOfFilePlayers> filePlayer;
};
