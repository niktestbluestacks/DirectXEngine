#pragma once
#ifndef _MODELCLASS_HPP_
#define _MODELCLASS_HPP_

// Includes
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>

// My Includes
#include "TextureClass.hpp"


// Class name : ModelClass
class ModelClass {
private:
    struct VertexType {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texture;
        DirectX::XMFLOAT3 normal;
    };

    struct ModelType {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

    /*struct TempVertexType {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

    struct VectorType {
        float x, y, z;
    };*/
public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadModel(char*);
    void ReleaseModel();

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount;
    ModelType* m_model;
};
#endif

