#include "ShaderManagerClass.hpp"

using namespace DirectX;
using namespace std;

ShaderManagerClass::ShaderManagerClass() {
    m_TextureShader = nullptr;
    m_LightShader = nullptr;
    m_NormalMapShader = nullptr;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other) {}

ShaderManagerClass::~ShaderManagerClass() {}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd) {
    bool result;
    m_TextureShader = new TextureShaderClass;

    result = m_TextureShader->Initialize(device, hwnd);
    if (!result) {
        return false;
    }

    m_LightShader = new LightShaderClass;

    result = m_LightShader->Initialize(device, hwnd);
    if (!result) {
        return false;
    }

    m_NormalMapShader = new NormalMapShaderClass;

    result = m_NormalMapShader->Initialize(device, hwnd);
    if (!result) {
        return false;
    }

    return true;
}

void ShaderManagerClass::Shutdown() {
    if (m_NormalMapShader) {
        m_NormalMapShader->Shutdown();
        delete m_NormalMapShader;
        m_NormalMapShader = nullptr;
    }

    if (m_LightShader) {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = nullptr;
    }

    if (m_TextureShader) {
        m_TextureShader->Shutdown();
        delete m_TextureShader;
        m_TextureShader = nullptr;
    }

    return;
}

bool ShaderManagerClass::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount,
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
    ID3D11ShaderResourceView* texture) {

    bool result;


    result = m_TextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
    if (!result)
    {
        return false;
    }

    return true;
}


bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount,
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
    ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor) {

    bool result;


    result = m_LightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix,
        texture, lightDirection, diffuseColor);
    if (!result) {
        return false;
    }

    return true;
}


bool ShaderManagerClass::RenderNormalMapShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
    ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor) {
    bool result;


    result = m_NormalMapShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix,
        colorTexture, normalTexture, lightDirection, diffuseColor);
    if (!result) {
        return false;
    }

    return true;
}