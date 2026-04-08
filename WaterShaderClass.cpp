#include "WaterShaderClass.hpp"
#include <cstdint>

typedef uint64_t ui64;

using namespace DirectX;
using namespace std;

WaterShaderClass::WaterShaderClass() {
    m_vertexShader = nullptr;
    m_pixelShader = nullptr;
    m_layout = nullptr;
    m_sampleState = nullptr;
    m_matrixBuffer = nullptr;
    m_reflectionBuffer = nullptr;
    m_waterBuffer = nullptr;
}

WaterShaderClass::WaterShaderClass(const WaterShaderClass& other) {}

WaterShaderClass::~WaterShaderClass() {}

bool WaterShaderClass::Initialize(ID3D11Device* device, HWND hwnd) {
    bool result;
    wchar_t vsFilename[128];
    wchar_t psFilename[128];
    int error;
    error = wcscpy_s(vsFilename, 128, L"../DirectXEngine/watervs.hlsl");
    if (error != 0) {
        return false;
    }

    error = wcscpy_s(psFilename, 128, L"../DirectXEngine/waterps.hlsl");
    if (error != 0) {
        return false;
    }

    result = InitializeShader(device, hwnd, vsFilename, psFilename);
    if (!result) {
        return false;
    }

    return true;
}

void WaterShaderClass::Shutdown() {
    ShutdownShader();

    return;
}

bool WaterShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
    XMMATRIX reflectionMatrix, ID3D11ShaderResourceView* reflectionTexture, ID3D11ShaderResourceView* refractionTexture,
    ID3D11ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale) {

    bool result;


    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix, reflectionTexture,
        refractionTexture, normalTexture, waterTranslation, reflectRefractScale);
    if (!result) {
        return false;
    }

    RenderShader(deviceContext, indexCount);

    return true;
}

bool WaterShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) {
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC reflectionBufferDesc;
    D3D11_BUFFER_DESC waterBufferDesc;


    errorMessage = nullptr;
    vertexShaderBuffer = nullptr;
    pixelShaderBuffer = nullptr;


    result = D3DCompileFromFile(vsFilename, nullptr, nullptr, "WaterVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &vertexShaderBuffer, &errorMessage);
    if (FAILED(result)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        else {
            MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }

    result = D3DCompileFromFile(psFilename, nullptr, nullptr, "WaterPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &pixelShaderBuffer, &errorMessage);
    if (FAILED(result)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        }
        else {
            MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }

    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
        nullptr, &m_vertexShader);
    if (FAILED(result)) {
        return false;
    }

    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
        nullptr, &m_pixelShader);
    if (FAILED(result)) {
        return false;
    }

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result)) {
        return false;
    }

    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;

    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
    if (FAILED(result)) {
        return false;
    }

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result)) {
        return false;
    }

    reflectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    reflectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
    reflectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    reflectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    reflectionBufferDesc.MiscFlags = 0;
    reflectionBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&reflectionBufferDesc, nullptr, &m_reflectionBuffer);
    if (FAILED(result)) {
        return false;
    }

    waterBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    waterBufferDesc.ByteWidth = sizeof(WaterBufferType);
    waterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    waterBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    waterBufferDesc.MiscFlags = 0;
    waterBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&waterBufferDesc, nullptr, &m_waterBuffer);
    if (FAILED(result)) {
        return false;
    }

    return true;
}

void WaterShaderClass::ShutdownShader() {
    if (m_waterBuffer) {
        m_waterBuffer->Release();
        m_waterBuffer = nullptr;
    }

    if (m_reflectionBuffer) {
        m_reflectionBuffer->Release();
        m_reflectionBuffer = nullptr;
    }

    if (m_sampleState) {
        m_sampleState->Release();
        m_sampleState = nullptr;
    }

    if (m_matrixBuffer) {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }

    if (m_layout) {
        m_layout->Release();
        m_layout = nullptr;
    }

    if (m_pixelShader) {
        m_pixelShader->Release();
        m_pixelShader = nullptr;
    }

    if (m_vertexShader) {
        m_vertexShader->Release();
        m_vertexShader = nullptr;
    }

    return;
}


void WaterShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename) {
    char* compileErrors;
    ui64 bufferSize, i;
    ofstream fout;


    compileErrors = static_cast <char*> (errorMessage->GetBufferPointer());

    bufferSize = errorMessage->GetBufferSize();

    fout.open("shader-error.txt");

    for (i = 0; i < bufferSize; i++) {
        fout << compileErrors[i];
    }

    fout.close();

    errorMessage->Release();
    errorMessage = nullptr;

    MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

    return;
}

bool WaterShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX reflectionMatrix,
    ID3D11ShaderResourceView* reflectionTexture, ID3D11ShaderResourceView* refractionTexture, ID3D11ShaderResourceView* normalTexture,
    float waterTranslation, float reflectRefractScale) {

    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;
    ReflectionBufferType* dataPtr2;
    WaterBufferType* dataPtr3;

    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);
    reflectionMatrix = XMMatrixTranspose(reflectionMatrix);

    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    dataPtr = static_cast <MatrixBufferType*> (mappedResource.pData);

    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    deviceContext->Unmap(m_matrixBuffer, 0);

    bufferNumber = 0;

    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    result = deviceContext->Map(m_reflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    dataPtr2 = static_cast <ReflectionBufferType*> (mappedResource.pData);

    dataPtr2->reflection = reflectionMatrix;

    deviceContext->Unmap(m_reflectionBuffer, 0);

    bufferNumber = 1;

    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_reflectionBuffer);

    deviceContext->PSSetShaderResources(0, 1, &reflectionTexture);

    deviceContext->PSSetShaderResources(1, 1, &refractionTexture);

    deviceContext->PSSetShaderResources(2, 1, &normalTexture);

    result = deviceContext->Map(m_waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    dataPtr3 = static_cast <WaterBufferType*> (mappedResource.pData);

    dataPtr3->waterTranslation = waterTranslation;
    dataPtr3->reflectRefractScale = reflectRefractScale;
    dataPtr3->padding = XMFLOAT2(0.0f, 0.0f);

    deviceContext->Unmap(m_waterBuffer, 0);

    bufferNumber = 0;

    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_waterBuffer);

    return true;
}

void WaterShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {
    deviceContext->IASetInputLayout(m_layout);

    deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
    deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

    deviceContext->PSSetSamplers(0, 1, &m_sampleState);

    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}

