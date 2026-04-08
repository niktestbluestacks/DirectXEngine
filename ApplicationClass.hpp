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
#include "OrthoWindowClass.hpp"
#include "FadeShaderClass.hpp"
#include "TimerClass.hpp"

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
    //bool RenderRefractionToTexture();
    //bool RenderReflectionToTexture();
    bool RenderSceneToTexture(float);
    bool Render(float);
    //bool Render();

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    TextureShaderClass* m_TextureShader;
    ModelClass* m_Model;
    RenderTextureClass* m_RenderTexture;
    OrthoWindowClass* m_FullScreenWindow;
    FadeShaderClass* m_FadeShader;
    TimerClass* m_Timer;
    float m_accumulatedTime, m_fadeInTime;
};
#endif // _APPLICATIONCLASS_HPP_

