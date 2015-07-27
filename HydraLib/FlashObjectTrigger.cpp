#include "stdafx.h"
#include "FlashObjectTrigger.h"


FlashObjectTrigger::FlashObjectTrigger(EventTriggeredDelegate ^eventTriggered) :
	_eventTriggered(eventTriggered)
{

}

void FlashObjectTrigger::trigger() const
{
	EventTriggeredDelegate ^eventTriggered = _eventTriggered;
	eventTriggered();
}