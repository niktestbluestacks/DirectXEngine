#pragma once
#ifndef _REFLECTIONSHADERCLASS_HPP_
#define _REFLECTIONSHADERCLASS_HPP_

// Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

// Class ReflectionShaderClass
class ReflectionShaderClass {
private:
    struct MatrixBufferType {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };
    struct ReflectionBufferType {
        DirectX::XMMATRIX reflectionMatrix;
    };

public:
    ReflectionShaderClass();
    ReflectionShaderClass(const ReflectionShaderClass&);
    ~ReflectionShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
        ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, DirectX::XMMATRIX);

private:
    bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
        ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, DirectX::XMMATRIX);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_reflectionBuffer;
};
#endif // _REFLECTIONSHADERCLASS_HPP_