#include "SpriteClass.hpp"

using namespace DirectX;
using namespace std;

SpriteClass::SpriteClass() {
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_Textures = nullptr;
}


SpriteClass::SpriteClass(const SpriteClass& other) {}


SpriteClass::~SpriteClass() {}

bool SpriteClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight,
	char* textureFilename, int renderX, int renderY) {
	bool result;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_renderX = renderX;
	m_renderY = renderY;

	m_frameTime = 0.0f;

	result = InitializeBuffers(device);
	if (!result) {
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) {
		return false;
	}

	return true;
}

void SpriteClass::Shutdown() {

	ReleaseTexture();
	ShutdownBuffers();
	return;
}

bool SpriteClass::Render(ID3D11DeviceContext* deviceContext) {
	bool result;

	result = UpdateBuffers(deviceContext);
	if (!result) {
		return false;
	}
	RenderBuffers(deviceContext);

	return true;
}

void SpriteClass::Update(float frameTime) {
	m_frameTime += frameTime;
	if (m_frameTime >= m_cycleTime) {
		m_frameTime -= m_cycleTime;
		m_currentTexture++;
		if (m_currentTexture >= m_textureCount) {
			m_currentTexture = 0;
		}
	}
}

int SpriteClass::GetIndexCount() {
	return m_indexCount;
}

ID3D11ShaderResourceView* SpriteClass::GetTexture() {
	return m_Textures[m_currentTexture].GetTexture();
}

bool SpriteClass::InitializeBuffers(ID3D11Device* device) {
	VertexType* vertices;
	unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_prevPosX = -1;
	m_prevPosY = -1;


	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];

	indices = new unsigned int[m_indexCount];

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (int i = 0; i < m_indexCount; i++) {
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void SpriteClass::ShutdownBuffers() {
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

bool SpriteClass::UpdateBuffers(ID3D11DeviceContext* deviceContent) {
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* dataPtr;
	HRESULT result;


	if ((m_prevPosX == m_renderX) && (m_prevPosY == m_renderY)) {
		return true;
	}

	// If the rendering location has changed then store the new position and update the vertex buffer.
	m_prevPosX = m_renderX;
	m_prevPosY = m_renderY;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Calculate the screen coordinates of the left side of the bitmap.
	left = static_cast <float>((m_screenWidth / 2) * -1) + static_cast <float> (m_renderX);

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + static_cast <float> (m_bitmapWidth);

	// Calculate the screen coordinates of the top of the bitmap.
	top = static_cast <float> (m_screenHeight / 2) - static_cast <float> (m_renderY);

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - static_cast <float> (m_bitmapHeight);


	// Load the vertex array with data.
	// First triangle.
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

	result = deviceContent->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	dataPtr = (VertexType*)mappedResource.pData;

	memcpy(dataPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	deviceContent->Unmap(m_vertexBuffer, 0);

	dataPtr = 0;

	delete[] vertices;
	vertices = 0;

	return true;
}


void SpriteClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool SpriteClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {
	char textureFilename[128];
	ifstream fin;
	int i, j;
	char input;
	bool result;


	// Open the sprite info data file.
	fin.open(filename);
	if (fin.fail()) {
		return false;
	}
	fin >> m_textureCount;

	m_Textures = new TextureClass[m_textureCount];

	fin.get(input);

	for (i = 0; i < m_textureCount; i++) {
		j = 0;
		fin.get(input);
		while (input != '\n') {
			textureFilename[j] = input;
			j++;
			fin.get(input);
		}
		textureFilename[j] = '\0';

		result = m_Textures[i].Initialize(device, deviceContext, textureFilename);
		if (!result) {
			return false;
		}
	}

	fin >> m_cycleTime;

	m_cycleTime = m_cycleTime * 0.001f;

	fin.close();

	m_bitmapWidth = m_Textures[0].GetWidth();
	m_bitmapHeight = m_Textures[0].GetHeight();

	m_currentTexture = 0;

	return true;
}

void SpriteClass::ReleaseTexture() {
	if (m_Textures) {
		m_Textures->Shutdown();
		delete[] m_Textures;
		m_Textures = nullptr;
	}
	return;
}

void SpriteClass::SetRenderLocation(int x, int y) {
	m_renderX = x;
	m_renderY = y;
	return;
}