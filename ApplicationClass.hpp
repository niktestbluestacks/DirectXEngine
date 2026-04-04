#pragma once
#ifndef _APPLICATIONCLASS_HPP_
#define _APPLICATIONCLASS_HPP_

// My header files.
#include "D3DClass.hpp"
#include "InputClass.hpp"
#include "CameraClass.hpp"
#include "ModelClass.hpp"
#include "TextureShaderClass.hpp"
#include "RenderTextureClass.hpp"
#include "DisplayPlaneClass.hpp"


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
	bool Frame(InputClass*);
private:
	bool Render();
	bool RenderSceneToTexture(float rotation);

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
	RenderTextureClass* m_RenderTexture;
	DisplayPlaneClass* m_DisplayPlane;
};
#endif // _APPLICATIONCLASS_HPP_

