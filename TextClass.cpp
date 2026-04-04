#include "TextClass.hpp"

using namespace DirectX;
using namespace std;

TextClass::TextClass() {
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
}


TextClass::TextClass(const TextClass& other) {}


TextClass::~TextClass() {}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int maxLength,
    FontClass* Font, char* text,
    int positionX, int positionY, float red, float green, float blue) {
    bool result;


    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    m_maxLength = maxLength;

    result = InitializeBuffers(device, deviceContext, Font, text, positionX, positionY, red, green, blue);
    if (!result) {
        return false;
    }

    return true;
}

void TextClass::Shutdown() {
    ShutdownBuffers();

    return;
}

void TextClass::Render(ID3D11DeviceContext* deviceContext) {
    RenderBuffers(deviceContext);

    return;
}

int TextClass::GetIndexCount() {
    return m_indexCount;
}

bool TextClass::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
    FontClass* Font, char* text,
    int positionX, int positionY, float red, float green, float blue) {

    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    m_vertexCount = 6 * m_maxLength;
    m_indexCount = m_vertexCount;

    vertices = new VertexType[m_vertexCount];

    indices = new unsigned long[m_indexCount];

    memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

    for (i = 0; i < m_indexCount; i++) {
        indices[i] = i;
    }

    // Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result)) {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result)) {
        return false;
    }

    // Release the vertex array as it is no longer needed.
    delete[] vertices;
    vertices = 0;

    // Release the index array as it is no longer needed.
    delete[] indices;
    indices = 0;

    result = UpdateText(deviceContext, Font, text, positionX, positionY, red, green, blue);
    if (!result) {
        return false;
    }

    return true;
}

void TextClass::ShutdownBuffers() {
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

bool TextClass::UpdateText(ID3D11DeviceContext* deviceContext,
    FontClass* Font, char* text,
    int positionX, int positionY, float red, float green, float blue) {

    int numLetters;
    VertexType* vertices;
    float drawX, drawY;
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    VertexType* verticesPtr;

    m_pixelColor = XMFLOAT4(red, green, blue, 1.0f);

    numLetters = static_cast <int> (strlen(text));

    if (numLetters > m_maxLength) {
        return false;
    }

    vertices = new VertexType[m_vertexCount];

    memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

    drawX = static_cast <float> (((m_screenWidth / 2) * -1) + positionX);
    drawY = static_cast <float> ((m_screenHeight / 2) - positionY);

    Font->BuildVertexArray(static_cast <void*> (vertices), text, drawX, drawY);

    result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    verticesPtr = static_cast <VertexType*> (mappedResource.pData);

    memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

    deviceContext->Unmap(m_vertexBuffer, 0);

    delete[] vertices;
    vertices = nullptr;

    return true;
}

void TextClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
    unsigned int stride, offset;


    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}

XMFLOAT4 TextClass::GetPixelColor() {
    return m_pixelColor;
}

