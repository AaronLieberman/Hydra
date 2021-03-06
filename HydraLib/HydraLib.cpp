// This is the main DLL file.

#include "stdafx.h"

#include "HydraLib.h"
#include "ScopeLock.h"
#include "HydraController.h"

delegate void ControllerManagerSetupCallbackDelegate(sixenseUtils::ControllerManager::setup_step step);

public ref class HydraLibrary
{
private:

	GCHandle _controllerManagerSetupCallbackHandle;
	bool _controllerManagerScreenVisible = true;
	std::string *_controllerManagerTextString;
	Timer ^timer;

	HydraController ^_leftController = gcnew HydraController(sixenseUtils::ControllerManager::P1L);
	HydraController ^_rightController = gcnew HydraController(sixenseUtils::ControllerManager::P1R);

	void ControllerManagerSetupCallback(sixenseUtils::ControllerManager::setup_step step)
	{
		if (sixenseUtils::getTheControllerManager()->isMenuVisible())
		{
			// Turn on the flag that tells the graphics system to draw the instruction screen instead of the controller information. The game
			// should be paused at this time.
			_controllerManagerScreenVisible = true;

			// Ask the controller manager what the next instruction string should be.
			_controllerManagerTextString = new std::string;
			*_controllerManagerTextString = sixenseUtils::getTheControllerManager()->getStepString();

			// We could also load the supplied controllermanager textures using the filename: sixenseUtils::getTheControllerManager()->getTextureFileName();
		}
		else
		{
			// We're done with the setup, so hide the instruction screen.
			_controllerManagerScreenVisible = false;
			delete _controllerManagerTextString;
		}
	}

	void TimerTick(Object ^state)
	{
		sixenseSetActiveBase(0);
		sixenseAllControllerData acd;
		sixenseGetAllNewestData(&acd);
		sixenseUtils::getTheControllerManager()->update(&acd);

		_leftController->Update(acd);
		_rightController->Update(acd);

		// Now do the same thing but use event triggers to flash the object when a button is pressed, or when the 
		// controller moves to a certain height.

		//// First make a couple of BinaryEventSwitch that flash the object when the test parameter changes from false to true. Use a null trigger for when it transitions
		//// from true to false.
		//static sixenseUtils::EventSwitchBase *left_button_switch = new sixenseUtils::BinaryEventSwitch(new FlashObjectTrigger(gcnew EventTriggeredDelegate(this, &HydraLibrary::Triggered)), new sixenseUtils::NullEventTrigger);
		//static sixenseUtils::EventSwitchBase *right_button_switch = new sixenseUtils::BinaryEventSwitch(new FlashObjectTrigger(gcnew EventTriggeredDelegate(this, &HydraLibrary::Triggered)), new sixenseUtils::NullEventTrigger);
		//left_button_switch->test(((acd.controllers)[left_index].buttons & SIXENSE_BUTTON_4) ? 1.0f : 0.0f); // test against the current state of the 4 button
		//right_button_switch->test(((acd.controllers)[right_index].buttons & SIXENSE_BUTTON_4) ? 1.0f : 0.0f);

		//// First make a couple of BinaryEventSwitch that flash the object when the controller moves above a 200mm. Do nothing when it transitions back down.
		//// ValuatorEventSwitches can be used to test against any floating point value, including position, velocity, trigger positions, joystick positions, rotation angles, etc.
		//static sixenseUtils::EventSwitchBase *left_height_switch = new sixenseUtils::ValuatorEventSwitch(200.0f, new FlashObjectTrigger(gcnew EventTriggeredDelegate(this, &HydraLibrary::Triggered)), new sixenseUtils::NullEventTrigger);
		//static sixenseUtils::EventSwitchBase *right_height_switch = new sixenseUtils::ValuatorEventSwitch(200.0f, new FlashObjectTrigger(gcnew EventTriggeredDelegate(this, &HydraLibrary::Triggered)), new sixenseUtils::NullEventTrigger);
		//left_button_switch->test((acd.controllers)[left_index].pos[1]); // test the y position (height)
		//right_button_switch->test((acd.controllers)[right_index].pos[1]);
	}

	//void Triggered()
	//{
	//	Console::WriteLine("triggered");
	//}

public:

	HydraLibrary()
	{
		// Init sixense
		sixenseInit();

		// Init the controller manager. This makes sure the controllers are present, assigned to left and right hands, and that
		// the hemisphere calibration is complete.
		sixenseUtils::getTheControllerManager()->setGameType(sixenseUtils::ControllerManager::ONE_PLAYER_TWO_CONTROLLER);

		ControllerManagerSetupCallbackDelegate ^callback = gcnew ControllerManagerSetupCallbackDelegate(this, &HydraLibrary::ControllerManagerSetupCallback);
		_controllerManagerSetupCallbackHandle = GCHandle::Alloc(callback);

		IntPtr ip = Marshal::GetFunctionPointerForDelegate(callback);
		sixenseUtils::getTheControllerManager()->registerSetupCallback((sixenseUtils::ControllerManager::setup_callback)ip.ToPointer());

		timer = gcnew Timer(gcnew TimerCallback(this, &HydraLibrary::TimerTick), nullptr, 0, 33);
	}

	property HydraController ^LeftController { HydraController ^get() { return _leftController; } }
	property HydraController ^RightController { HydraController ^get() { return _rightController; } }

	~HydraLibrary()
	{
		delete timer;
		delete _leftController;
		delete _rightController;
		_controllerManagerSetupCallbackHandle.Free();
		sixenseExit();
	}

};
