#pragma once
#include "ShaderManagerClass.hpp"
#ifndef _SHADERMANAGERCLASS_HPP_
#define _SHADERMANAGERCLASS_HPP_

// My includes
#include "TextureShaderClass.hpp"
#include "LightShaderClass.hpp"
#include "NormalMapShaderClass.hpp"

// Class name: ShaderManadgerClass
class ShaderManagerClass {
public:
    ShaderManagerClass();
    ShaderManagerClass(const ShaderManagerClass&);
    ~ShaderManagerClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool RenderTextureShader(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*);
    bool RenderLightShader(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
        ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT4);
    bool RenderNormalMapShader(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
        ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, DirectX::XMFLOAT3, DirectX::XMFLOAT4);

private:
    TextureShaderClass* m_TextureShader;
    LightShaderClass* m_LightShader;
    NormalMapShaderClass* m_NormalMapShader;
};
#endif // _SHADERMANAGERCLASS_HPP_

