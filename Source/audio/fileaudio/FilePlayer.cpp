/*
  ==============================================================================
	FilePlayer.cpp
  ==============================================================================
*/

#include "FilePlayer.h"

FilePlayer::FilePlayer() : thread("FilePlayThread")
{
	thread.startThread();
	//plugs the AudioTransportSource into the ResamplingAudioSource - this will allow pitch control
	resamplingAudioSource = std::make_unique<ResamplingAudioSource>(&audioTransportSource, false);
}

FilePlayer::~FilePlayer()
{
	//unloads the current file
	audioTransportSource.setSource(nullptr);
	thread.stopThread(100);
}

bool FilePlayer::isPlaying() const
{
	return audioTransportSource.isPlaying();
}

bool FilePlayer::isLooping()
{
	return currentAudioFileSource->isLooping();
}

void FilePlayer::setPlaying(bool newState)
{
	if (newState == true)
	{
		audioTransportSource.setPosition(0.0);
		audioTransportSource.start();
	}
	else
	{
		audioTransportSource.stop();
	}
}

void FilePlayer::setLooping(bool shouldLoop)
{
	currentAudioFileSource->setLooping(shouldLoop);
}

void FilePlayer::setGain(float g)
{
	audioTransportSource.setGain(g);
}

void FilePlayer::setPlaybackRate(double newRate)
{
	resamplingAudioSource->setResamplingRatio(newRate);
}

void FilePlayer::loadFile(const File& newFile)
{
	//stops playback
	setPlaying(false);
	//unloads the previous file source and deletes it
	audioTransportSource.setSource(nullptr);

	//creates a format manager and sets it up with the basic types (wav, ogg and aiff).
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	//if the file can be read, a reader is created and passed to the AudioFormatReaderSource
	if (auto reader = formatManager.createReaderFor(newFile))
	{
		//reader is passed to the AudioFormatReaderSource currentAudioFileSource
		//currentAudioFileSource is set to delete the reader when it goes out of scope - otherwise
		//the reader will not be deleted
		currentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);

		//currentAudioFileSource is plugged into audioTransportSource
		//it will buffer 32768 samples ahead
		audioTransportSource.setSource(currentAudioFileSource.get(),
										32768,
										&thread,
										reader->sampleRate);
	}
}

//AudioSource
void FilePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	resamplingAudioSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void FilePlayer::releaseResources()
{
	resamplingAudioSource->releaseResources();
}

void FilePlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	resamplingAudioSource->getNextAudioBlock(bufferToFill);
}