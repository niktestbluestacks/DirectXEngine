#pragma once
#ifndef _APPLICATIONCLASS_HPP_
#define _APPLICATIONCLASS_HPP_

// My header files.
#include "D3DClass.hpp"
#include "InputClass.hpp"
#include "CameraClass.hpp"
#include "ModelClass.hpp"
#include "DepthShaderClass.hpp"

// Global constants.
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

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
    //bool RenderRefractionToTexture();
    //bool RenderReflectionToTexture();
    //bool RenderSceneToTexture(float);
    //bool Render(float);
    bool Render();

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_Model;
    DepthShaderClass* m_DepthShader;
};
#endif // _APPLICATIONCLASS_HPP_

