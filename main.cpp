#include "SysteamClass.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass* System;
	bool result;

	// Create the system object.
	System = new SystemClass;
	if (!System) {
		return 0;
	}
	result = System->Initialize();
	if (result) {
		System->Run();
	}
	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = nullptr;
	return 0;
}