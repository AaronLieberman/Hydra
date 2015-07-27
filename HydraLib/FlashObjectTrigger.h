#pragma once

#include "stdafx.h"

delegate void EventTriggeredDelegate();

// EventTriggers are very flexible objects that can be used to check for transitions of controller state including 
// buttons being pressed, controllers moving a certain distance, or exceeding a certain velocity.
class FlashObjectTrigger : public sixenseUtils::EventTriggerBase
{
private:

	gcroot<EventTriggeredDelegate^> _eventTriggered;
	int enable_for_frames;

public:

	FlashObjectTrigger(EventTriggeredDelegate ^eventTriggered);

	virtual void trigger() const;

};
