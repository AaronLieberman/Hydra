#pragma once

// TODO: split into .h/.cpp file after this stabilizes
public ref class HydraController
{
private:

	Object ^_lockObject = gcnew Object();
	sixenseUtils::ControllerManager::controller_desc _desc;
	sixenseUtils::ButtonStates *_lastButtonStates;

internal:

	void Update(sixenseAllControllerData &acd)
	{
		ScopeLock lock(_lockObject);

		int left_index = sixenseUtils::getTheControllerManager()->getIndex(_desc);

		_lastButtonStates->update(&acd.controllers[left_index]);
	}

public:

	HydraController(sixenseUtils::ControllerManager::controller_desc desc) :
		_desc(desc)
	{
		_lastButtonStates = new sixenseUtils::ButtonStates;
	}

	~HydraController()
	{
		delete _lastButtonStates;
	}

	bool ButtonJustPressed()
	{
		ScopeLock lock(_lockObject);
		return _lastButtonStates->buttonJustPressed(SIXENSE_BUTTON_1);
	}

	bool TriggerJustPressed()
	{
		ScopeLock lock(_lockObject);
		return _lastButtonStates->triggerJustPressed();
	}

};
