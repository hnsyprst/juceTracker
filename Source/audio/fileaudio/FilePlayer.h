/*
  ==============================================================================
	FilePlayer.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/** Streams audio from a file. Streams audio using an AudioFormatReaderSource into an AudioTransportSource,
	into a ResamplingAudioSource to allow pitch control through changes in sampling rate. Files are
	streamed on their own thread. */

class FilePlayer		:	public AudioSource
{
public:
	/** Constructor. */
	FilePlayer();

	/** Destructor. */
	~FilePlayer();

	/** Gets the current playback state of the AudioTransportSource.
		@return	bool of the current playback state of the AudioTransportSource
		@see	setPlaying */
	bool isPlaying() const;

	/** Gets the current looping state of the AudioFormatReaderSource.
		@return	bool of the current looping state of the AudioFormatReaderSource
		@see	setLooping */
	bool isLooping();

	/** Starts or stops playback of the loaded file.
		@param	bool of the new playback state - true is play, false is stop
		@see	isPlaying */
	void setPlaying(bool newState);

	/** Sets the looping state of the AudioFormatReaderSource - default is do not loop.
		@param	bool of the new looping state - true is loop, false is do not loop
		@see	isLooping */
	void setLooping(bool shouldLoop);

	/** Sets the gain of the AudioTransportSource.
		@param	float new gain value */
	void setGain(float g);

	/** Sets the resampling rate of the ResamplingAudioSource.
		@param	double new resampling rate */
	void setPlaybackRate(double newRate);

	/** Loads the specified file into the AudioTransportSource via AudioFormatReaderSource.
		@param File to be streamed */
	void loadFile(const File& newFile);

	//AudioSource
	/** Overridden function inherited from AudioSource. Calls prepareToPlay() on the ResamplingAudioSource, passing
		the same variables as passed to this method.
		@param	int number of samples the ResamplingAudioSource is expected to supply each time its getNextAudioBlock()
		method is called
		@param double sample rate the ResamplingAudioSource will be used at */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	/** Overridden function inherited from AudioSource. Calls releaseResources() on the ResamplingAudioSource, passing
		the same variables as passed to this method. */
	void releaseResources() override;
	/** Overridden function inherited from AudioSource. Calls getNextAudioBlock() on the ResamplingAudioSource, passing
		the same variables as passed to this method.
		@param	reference to the next block of audio data */
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

private:
	AudioTransportSource audioTransportSource;						
	TimeSliceThread thread;
	std::unique_ptr<ResamplingAudioSource> resamplingAudioSource;
	std::unique_ptr<AudioFormatReaderSource> currentAudioFileSource;
};
