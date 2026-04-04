#pragma once
#ifndef _APPLICATIONCLASS_HPP_
#define _APPLICATIONCLASS_HPP_

// My header files.
#include "D3DClass.hpp"
#include "CameraClass.hpp"
#include "TextureShaderClass.hpp"
#include "BitmapClass.hpp"


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
	BitmapClass* m_Model;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
};
#endif // _APPLICATIONCLASS_HPP_

