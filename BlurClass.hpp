#pragma once
#ifndef _BLURCLASS_HPP_
#define _BLURCLASS_HPP_

// My includes
#include "D3DClass.hpp"
#include "CameraClass.hpp"
#include "RenderTextureClass.hpp"
#include "OrthoWindowClass.hpp"
#include "TextureShaderClass.hpp"
#include "BlurShaderClass.hpp"

// Class name: BlurClass
class BlurClass {
public:
    BlurClass();
    BlurClass(const BlurClass&);
    ~BlurClass();

    bool Initialize(D3DClass*, int, int, float, float, int, int);
    void Shutdown();

    bool BlurTexture(D3DClass*, CameraClass*, RenderTextureClass*, TextureShaderClass*, BlurShaderClass*);

private:
    RenderTextureClass* m_DownSampleTexure1, * m_DownSampleTexure2;
    OrthoWindowClass* m_DownSampleWindow, * m_UpSampleWindow;
    int m_downSampleWidth, m_downSampleHeight;
};
#endif // _BLURCLASS_HPP_
