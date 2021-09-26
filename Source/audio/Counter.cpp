/*
  ==============================================================================
	Counter.cpp
  ==============================================================================
*/

#include "Counter.h"

Counter::Counter(int numRows)		:	Thread("CounterThread"),
										runState(false),
										rows(numRows)
{
	startThread();
}

Counter::~Counter()
{
	stopThread(10);
}

bool Counter::isRunning()
{
	return runState;
}

int Counter::calculateWaitTimeFromBPM(int bpm)
{
	//bpm divided by 60,000 = time in milliseconds for each sixteenth note
	return (60000 / bpm) / 4;
}

void Counter::setListener(Counter::Listener* newListener)
{
	listener = newListener;
}

void Counter::setRunState(bool newState, int bpm)
{
	if (newState == true)
	{
		//set the counter back to the start
		counter = 0;
		//calculate counter speed
		waitTimeFromBPM = calculateWaitTimeFromBPM(bpm);
		//start counting
		runState = true;
	}
	else runState = false;
}

void Counter::run()
{
	//reset the counter
	counter = 0;
	while (!threadShouldExit())
	{
		if (runState == true)
		{
			//if the counter reaches or goes above the number of rows to count
			if (counter >= rows)
			{
				//reset the counter
				counter = 0;
			}
			//if a listener has been passed to the counter object
			if (listener != nullptr)
			{
				//broadcast the current counter value
				listener->counterChanged(counter);
			}
			//increment the counter
			counter++;
			//wait the specified time
			wait(waitTimeFromBPM);
		}
	}
}