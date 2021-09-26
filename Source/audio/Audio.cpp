/*
  ==============================================================================
	Audio.cpp
  ==============================================================================
*/

#include "Audio.h"

Audio::Audio()		:	tempo(130),
						sampleCounter(0),
						rowCounter(0),
						runState(false)
{
	//adds each FilePlayer as an input to the MixerAudioSource
	for (int i = 0; i < Audio::NumberOfFilePlayers; i++)
	{
		mixerAudioSource.addInputSource(&filePlayer[i], false);
	}

	//adds the MixerAudioSource as an input to the AudioSourcePlayer that will be used for output
	audioSourcePlayer.setSource(&mixerAudioSource);

	//sets the audio output device to the default device, printing an errorMessage to console if no audio devices are available
	auto errorMessage = audioDeviceManager.initialiseWithDefaultDevices(1, 2);
	if (!errorMessage.isEmpty())
	{
		DBG(errorMessage);
	}
	audioDeviceManager.addAudioCallback(this);
}

Audio::~Audio()
{
	//following lines perform cleanup for when Audio goes out of scope (i.e. when application closed)
	//removes audio and midi callbacks
	audioDeviceManager.removeAudioCallback(this);

	//remove the MixerAudioSource from the AudioSourcePlayer used for output, ending audio
	audioSourcePlayer.setSource(nullptr);
	//removes all inputs from the MixerAudioSource
	mixerAudioSource.removeAllInputs();
}

FilePlayer* Audio::getFilePlayer(int index)
{
	//check if index passed is in range
	if (index < filePlayer.size())
	{
		return &filePlayer[index];
	}
	else
	{
		DBG("filePlayer index out of range");
		return nullptr;
	}
}

std::array<FilePlayer, Audio::NumberOfFilePlayers>& Audio::getFilePlayerArray()
{
	return filePlayer;
}

void Audio::setBpm(int bpm)
{
	//bpm divided by 60,000 = time in milliseconds for each sixteenth note
	tempo = (60000 / bpm) / 4;
}

void Audio::setRunState(bool rs)
{
	runState = rs;
}

void Audio::setTrackerCellGuiArray(std::array<std::array<TrackerCellGui, 4>, 64>& tcga)
{
	//points the pointer (*trackerCellGuiArray) to the address of the first element in the
	//2D array tcga - this allows standard array access syntax later on in the audioDeviceIOCallback
	trackerCellGuiArray = &tcga[0];
}

void Audio::audioDeviceIOCallback(const float** inputChannelData,
	int numInputChannels,
	float** outputChannelData,
	int numOutputChannels,
	int numSamples)
{
	//get the audio from our file player - player puts samples in the output buffer
	audioSourcePlayer.audioDeviceIOCallback(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
	
	sampleRate = audioDeviceManager.getAudioDeviceSetup().sampleRate;

	//all audio processing is done here
	const float* inL = inputChannelData[0];

	float *outL = outputChannelData[0];
	float *outR = outputChannelData[1];

	while (numSamples--)
	{
		//if the runState of the tracker has been set to true, begin iterating through the TrackerCellGuis held in each row of the
		//tracker interface
		if (runState)
		{
			/*	sampleRate / 1000 = number of samples per millisecond
				number of samples per millisecond * rate at which the events held in TrackerCellGuis will be read in milliseconds
				= number of samples to count before reading a new TrackerCellGui event */
			if (sampleCounter >= ((sampleRate / 1000) * tempo))
			{
				//for each TrackerCellGui in the current row
				for (int col = 0; col < trackerCellGuiArray[0].size(); col++)
				{
					//get the sampleNumber from the current TrackerCellGui being read
					int sampleNumber = trackerCellGuiArray[rowCounter][col].getEventInfo()[1].getIntValue();

					//sampleNumber -1 is used to represent an invalid / unset sample - ignore this cell if sampleNumber is -1
					if (sampleNumber != -1)
					{
						//get the pitch and gain from the current TrackerCellGui being read
						double pitch = trackerCellGuiArray[rowCounter][col].getEventInfo()[0].getDoubleValue();
						float gain = trackerCellGuiArray[rowCounter][col].getEventInfo()[2].getFloatValue();

						//pass these values to the relevant FilePlayer
						filePlayer[sampleNumber].setPlaybackRate(pitch);
						filePlayer[sampleNumber].setGain(gain);
						filePlayer[sampleNumber].setPlaying(true);
					}
				}

				//increment the row counter
				rowCounter++;
				//if the row counter is equal to the number of rows in the tracker interface
				if (rowCounter == 63)
				{
					//reset the row counter
					rowCounter = 0;
				}
				//reset the sample counter
				sampleCounter = 0;
			}
			//increment the sample counter
			sampleCounter++;
		}
		//if the run state of the tracker is false, reset the sample and row counters
		else
		{
			sampleCounter = 0;
			rowCounter = 0;
		}
		float fileOutL = *outL;
		float fileOutR = *outR;

		*outL = fileOutL;
		*outR = fileOutR;

		inL++;
		outL++;
		outR++;
	}
}

void Audio::audioDeviceAboutToStart(AudioIODevice* device)
{
	audioSourcePlayer.audioDeviceAboutToStart(device);
}

void Audio::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
}
