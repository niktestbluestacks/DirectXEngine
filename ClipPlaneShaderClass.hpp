#pragma once
#ifndef _CLIPPLANESHADERCLASS_HPP_
#define _CLIPPLANESHADERCLASS_HPP_

// Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

// Class name: ClipPlaneShaderClass
class ClipPlaneShaderClass {
private:
    struct MatrixBufferType {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    struct ClipPlaneBufferType {
        DirectX::XMFLOAT4 clipPlane;
    };

public:
    ClipPlaneShaderClass();
    ClipPlaneShaderClass(const ClipPlaneShaderClass&);
    ~ClipPlaneShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int,
        DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT4);

private:
    bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*,
        DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*, DirectX::XMFLOAT4);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_clipPlaneBuffer;
};
#endif // _CLIPPLANESHADERCLASS_HPP_
