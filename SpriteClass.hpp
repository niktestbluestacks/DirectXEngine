#pragma once
#ifndef _SPRITECLASS_HPP_
#define _SPRITECLASS_HPP_

// Includes
#include <DirectXMath.h>
#include <fstream>
// My own includes
#include "TextureClass.hpp"

// Class name: SpriteClass
class SpriteClass {
private:
	struct VertexType {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
public:
    SpriteClass();
    SpriteClass(const SpriteClass&);
    ~SpriteClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, char*, int, int);
    void Shutdown();
    bool Render(ID3D11DeviceContext*);

    void Update(float);

    int GetIndexCount();
    ID3D11ShaderResourceView* GetTexture();

    void SetRenderLocation(int, int);

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    bool UpdateBuffers(ID3D11DeviceContext*);
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
    void ReleaseTexture();

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount, m_screenWidth, m_screenHeight, m_bitmapWidth, m_bitmapHeight, m_renderX, m_renderY, m_prevPosX, m_prevPosY;
    TextureClass* m_Textures;
    float m_frameTime, m_cycleTime;
    int m_currentTexture, m_textureCount;
};
#endif // _SPRITECLASS_HPP_

