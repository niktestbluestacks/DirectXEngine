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
        DirectX::XMFLOAT3 tangent;
        DirectX::XMFLOAT3 binormal;
    };

    struct ModelType {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
        float tx, ty, tz;
        float bx, by, bz;
    };

    struct TempVertexType {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

    struct VectorType {
        float x, y, z;
    };

public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount();
    ID3D11ShaderResourceView* GetTexture(int);

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
    void ReleaseTextures();

    bool LoadModel(char*);
    void ReleaseModel();

    void CalculateModelVectors();
    void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount;
    TextureClass* m_Textures;
    ModelType* m_model;
};
#endif

