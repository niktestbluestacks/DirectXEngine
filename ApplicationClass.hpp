#pragma once
#ifndef _APPLICATIONCLASS_HPP_
#define _APPLICATIONCLASS_HPP_

// My header files.
#include "D3DClass.hpp"
#include "InputClass.hpp"
#include "CameraClass.hpp"
#include "ModelClass.hpp"
#include "LightClass.hpp"
#include "RenderTextureClass.hpp"
#include "LightShaderClass.hpp"
#include "RefractionShaderClass.hpp"
#include "WaterShaderClass.hpp"

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
    bool RenderRefractionToTexture();
    bool RenderReflectionToTexture();
    bool Render();

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_GroundModel, * m_WallModel, * m_BathModel, * m_WaterModel;
    LightClass* m_Light;
    RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;
    LightShaderClass* m_LightShader;
    RefractionShaderClass* m_RefractionShader;
    WaterShaderClass* m_WaterShader;
    float m_waterHeight, m_waterTranslation;
};
#endif // _APPLICATIONCLASS_HPP_

