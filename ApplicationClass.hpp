#pragma once
#ifndef _APPLICATIONCLASS_HPP_
#define _APPLICATIONCLASS_HPP_

// My header files.
#include "D3DClass.hpp"
#include "CameraClass.hpp"
#include "ModelClass.hpp"
#include "LightShaderClass.hpp"
#include "LightClass.hpp"

// Global constants.
const bool FULL_SCREEN = true;
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
	bool Render(float);
private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
};
#endif // _APPLICATIONCLASS_HPP_

