#include "OrthoWindowClass.hpp"

using namespace DirectX;

OrthoWindowClass::OrthoWindowClass() {
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
}

OrthoWindowClass::OrthoWindowClass(const OrthoWindowClass& other) {}

OrthoWindowClass::~OrthoWindowClass() {}

bool OrthoWindowClass::Initialize(ID3D11Device* device, int windowWidth, int windowHeight) {
    bool result;


    result = InitializeBuffers(device, windowWidth, windowHeight);
    if (!result) {
        return false;
    }

    return true;
}

void OrthoWindowClass::Shutdown() {
    ShutdownBuffers();

    return;
}

void OrthoWindowClass::Render(ID3D11DeviceContext* deviceContext) {
    RenderBuffers(deviceContext);

    return;
}

int OrthoWindowClass::GetIndexCount() {
    return m_indexCount;
}

bool OrthoWindowClass::InitializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight) {
    float left, right, top, bottom;
    VertexType* vertices;
    unsigned int* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    left = static_cast <float> ((windowWidth / 2) * -1);

    right = left + static_cast <float> (windowWidth);

    top = static_cast <float> (windowHeight / 2);

    bottom = top - static_cast <float> (windowHeight);

    m_vertexCount = 6;

    m_indexCount = m_vertexCount;

    vertices = new VertexType[m_vertexCount];

    indices = new unsigned int[m_indexCount];

    // First triangl
    vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
    vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

    vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
    vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

    vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
    vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

    // Second triangle.
    vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
    vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

    vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
    vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

    vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
    vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

    for (i = 0; i < m_indexCount; i++) {
        indices[i] = i;
    }
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result)) {
        return false;
    }

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result)) {
        return false;
    }
    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

void OrthoWindowClass::ShutdownBuffers() {
    if (m_indexBuffer) {
        m_indexBuffer->Release();
        m_indexBuffer = nullptr;
    }

    if (m_vertexBuffer) {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }

    return;
}

void OrthoWindowClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
    unsigned int stride;
    unsigned int offset;


    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}