#include "SysteamClass.hpp"
#include "Logger.hpp"
#include <iostream>

/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass* System;
	bool result;
	dxed::Logger logger(dxed::Logger::LogLevel::Error);

	std::cin.get();

	logger.logInfo("Another day another bug");
	logger.logInfo("-----------------------");

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
	logger.logInfo("Shutting down the system...");
	System->Shutdown();
	delete System;
	System = nullptr;
	logger.logInfo("System shutdown successfully!");
	return 0;
}*/

auto main(void) -> signed {
	SystemClass* System;
	bool result;
	dxed::Logger logger(dxed::Logger::LogLevel::Info);

	logger.logInfo("Another day another bug");
	logger.logInfo("-----------------------");

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
	logger.logInfo("Shutting down the system...");
	System->Shutdown();
	delete System;
	System = nullptr;
	logger.logInfo("System shutdown successfully!");
	return 0;
}