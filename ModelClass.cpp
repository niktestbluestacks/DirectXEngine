#include "ModelClass.hpp"

using namespace DirectX;
using namespace std;

ModelClass::ModelClass() {
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
    m_Textures = nullptr;
    m_model = nullptr;
}

ModelClass::ModelClass(const ModelClass& other) {}


ModelClass::~ModelClass() {}


bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
    char* modelFilename, char* textureFilename1, char* textureFilename2) {

    bool result;


    result = LoadModel(modelFilename);
    if (!result) {
        return false;
    }

    result = InitializeBuffers(device);
    if (!result) {
        return false;
    }

    result = LoadTextures(device, deviceContext, textureFilename1, textureFilename2);
    if (!result) {
        return false;
    }

    return true;
}


void ModelClass::Shutdown() {
    ReleaseTextures();
    ShutdownBuffers();
    ReleaseModel();
    return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext) {
    RenderBuffers(deviceContext);
    return;
}


int ModelClass::GetIndexCount() {
    return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture(int index) {
    return m_Textures[index].GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device) {
    VertexType* vertices;
    unsigned int* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    vertices = new VertexType[m_vertexCount];

    indices = new unsigned int[m_indexCount];

    for (i = 0; i < m_vertexCount; i++) {
        vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
        vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
        vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

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

void ModelClass::ShutdownBuffers() {
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


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}

bool ModelClass::LoadTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
    char* textureFilename1, char* textureFilename2) {
    bool result;


    m_Textures = new TextureClass[2];

    result = m_Textures[0].Initialize(device, deviceContext, textureFilename1);
    if (!result) {
        return false;
    }

    result = m_Textures[1].Initialize(device, deviceContext, textureFilename2);
    if (!result)
    {
        return false;
    }

    return true;
}

void ModelClass::ReleaseTextures() {
    if (m_Textures) {
        m_Textures[0].Shutdown();
        m_Textures[1].Shutdown();

        delete[] m_Textures;
        m_Textures = nullptr;
    }

    return;
}


bool ModelClass::LoadModel(char* filename) {
    ifstream fin;
    char input;
    int i;


    fin.open(filename);

    if (fin.fail()) {
        return false;
    }

    fin.get(input);
    while (input != ':') {
        fin.get(input);
    }

    fin >> m_vertexCount;

    m_indexCount = m_vertexCount;

    m_model = new ModelType[m_vertexCount];

    fin.get(input);
    while (input != ':') {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    for (i = 0; i < m_vertexCount; i++) {
        fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
        fin >> m_model[i].tu >> m_model[i].tv;
        fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
    }

    fin.close();

    return true;
}


void ModelClass::ReleaseModel() {
    if (m_model) {
        delete[] m_model;
        m_model = nullptr;
    }

    return;
}
