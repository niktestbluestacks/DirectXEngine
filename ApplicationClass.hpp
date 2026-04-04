#pragma once
#ifndef _APPLICATIONCLASS_HPP_
#define _APPLICATIONCLASS_HPP_

// My header files.
#include "D3DClass.hpp"
#include "CameraClass.hpp"
#include "FontShaderClass.hpp"
#include "FontClass.hpp"
#include "TextClass.hpp"


// Global constants.
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

// Class name: ApplicationClass

class ApplicationClass {
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
private:
	bool Render();
private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	FontShaderClass* m_FontShader;
	FontClass* m_Font;
	TextClass* m_TextString1, * m_TextString2;
};
#endif // _APPLICATIONCLASS_HPP_

