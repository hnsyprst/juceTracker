/*
  ==============================================================================
	Counter.h
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/** Counts from 0 to value passed in constructor. Adds 1 to counter each 16th note at the speed passed by
	setRunState. Contains a Listener class for recieving broadcasted changes to the counter. */

class Counter		:	public Thread
{
public:
	/** Constructor.
		@param int number of rows to count up to */
	Counter(int numRows);

	/** Destructor. */
	~Counter();

	/** Class for counter listeners to inherit. */
	class Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}

		/** Pure virtual function called when the timer has reached the next interval.
			@param int current counter value */
		virtual void counterChanged(int counterValue) = 0;
	};

	/** Gets the run state of the counter.
		@return	bool run state of the counter (true is counting, false not counting). */
	bool isRunning();

	/** Calculates the time to wait after each count in milliseconds from the given BPM.
		@param	int speed to count at in beats per minute.
		@return	int time to wait after each count in milliseconds. */
	int calculateWaitTimeFromBPM(int bpm);

	/** Creates a new listener object.
		@param	pointer to a Listener object. */
	void setListener(Counter::Listener* newListener);

	/** Sets the run state of the counter at the speed passed in beats per minute.
		@param	bool run state of the counter (true is counting, false not counting). 
		@param	int speed to count at in beats per minute. */
	void setRunState(bool newState, int bpm);
	
	//Thread
	/** Overridden function inherited from Thread. Counts from 0 to the specified number of rows
		at the specified BPM. */
	void run() override;

private:
	Counter::Listener* listener = nullptr;

	bool runState;
	int counter;
	int rows;
	int waitTimeFromBPM;
};
