#pragma once
#ifndef _BLURSHADERCLASS_HPP_
#define _BLURSHADERCLASS_HPP_

// Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

// Class name: BlueShaderClass
class BlurShaderClass {
private:
    struct MatrixBufferType {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    struct ScreenBufferType {
        float screenWidth;
        float screenHeight;
        float blurType;
        float padding;
    };

public:
    BlurShaderClass();
    BlurShaderClass(const BlurShaderClass&);
    ~BlurShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int,
        DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
        ID3D11ShaderResourceView*, int, int, float);

private:
    bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, 
        DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
        ID3D11ShaderResourceView*, int, int, float);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_screenBuffer;
};
#endif // _BLURSHADERCLASS_HPP_

