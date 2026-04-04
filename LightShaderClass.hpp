#pragma once
#ifndef _LIGHTSHADERCLASS_HPP_
#define _LIGHTSHADERCLASS_HPP_

// Globals
const int NUM_LIGHTS = 4;

// Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

// Class name: LightShaderClass
class LightShaderClass {
private:
	struct MatrixBufferType {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
    };

	struct LightColorBufferType {
		DirectX::XMFLOAT4 diffuseColor[NUM_LIGHTS];
	};

	struct LightPositionBufferType {
		DirectX::XMFLOAT4 lightPosition[NUM_LIGHTS];
	};
public:
    LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);

	void Shutdown();
	bool Render(ID3D11DeviceContext*, int,
		DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*,
		DirectX::XMFLOAT4[], DirectX::XMFLOAT4[]);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
		ID3D11ShaderResourceView*, DirectX::XMFLOAT4[], DirectX::XMFLOAT4[]);

	void RenderShader(ID3D11DeviceContext*, int);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;
};
#endif  // _LIGHTSHADERCLASS_HPP_
