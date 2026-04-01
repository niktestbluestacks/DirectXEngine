#pragma once
#ifndef _SYSTEAMCLASS_H_
#define _SYSTEAMCLASS_H_
#define WIN32_LEAN_AND_MEAN
// Windows Header Files:
#include <Windows.h>
// My Header Files:
#include "InputClass.hpp"
#include "ApplicationClass.hpp"

// class name: SystemClass
class SystemClass {
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// Function prototype for Windows.
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Globals.
static SystemClass* ApplicationHandle = 0;

#endif // _SYSTEAMCLASS_H_