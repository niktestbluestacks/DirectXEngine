#include "SysteamClass.hpp"

SystemClass::SystemClass() {
	m_Input = nullptr;
	m_Application = nullptr;
}

SystemClass::SystemClass(const SystemClass& other) {
}

SystemClass::~SystemClass() {
}

bool SystemClass::Initialize() {
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;

	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result) {
		return false;
	}


	m_Application = new ApplicationClass;

	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) {
		return false;
	}

	return true;
}

void SystemClass::Shutdown() {

	if (m_Application) {
		m_Application->Shutdown();
		delete m_Application;
		m_Application = nullptr;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = nullptr;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run() {
	MSG msg;
	bool done, result;
	ZeroMemory(&msg, sizeof(MSG));
	done = false;
	while (!done) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {
			result = Frame();
			if (!result) {
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame() {
	bool result;
	result = m_Input->Frame();
	if (!result) {
		return false;
	}

	result = m_Application->Frame(m_Input);

	if (!result) {
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(nullptr);

	// Give the application a name.
	m_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned int)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned int)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, m_hinstance, nullptr);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows() {
	ShowCursor(true);
	if (FULL_SCREEN) {
		ChangeDisplaySettings(nullptr, 0);
	}
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = nullptr;
	ApplicationHandle = nullptr;
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}