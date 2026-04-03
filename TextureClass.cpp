#include "TextureClass.hpp"

TextureClass::TextureClass() {
	m_targaData = nullptr;
	m_texture = nullptr;
	m_textureView = nullptr;
}


TextureClass::TextureClass(const TextureClass& other) {}


TextureClass::~TextureClass() {}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {
	bool result;
	result = LoadTarga32Bit(filename);
	if (!result) {
		return false;
	}
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	//D3D11_SUBRESOURCE_DATA subResourceData;
	//subResourceData.pSysMem = m_targaData;
	//subResourceData.SysMemPitch = m_width * 4; // 4 bytes per pixel
	//subResourceData.SysMemSlicePitch = 0;
	HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
	if (FAILED(hr)) {
		return false;
	}

	unsigned int rowPitch = (m_width * 4) * sizeof(unsigned char);
	deviceContext->UpdateSubresource(m_texture, 0, nullptr, m_targaData, rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hr)) {
		return false;
	}

	deviceContext->GenerateMips(m_textureView);

	delete[] m_targaData;
	m_targaData = nullptr;

	return true;
}

void TextureClass::Shutdown() {
	if (m_textureView) {
		m_textureView->Release();
		m_textureView = nullptr;
	}
	if (m_texture) {
		m_texture->Release();
		m_texture = nullptr;
	}
	if (m_targaData) {
		delete[] m_targaData;
		m_targaData = nullptr;
	}
	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture() {
	return m_textureView;
}

bool TextureClass::LoadTarga32Bit(char* filename) {
	FILE* filePtr;
	TargaHeader targaFileHeader;
	unsigned int imageSize, index, i, j, k;
	unsigned char* targaImage;
	int error;

	filePtr = nullptr;
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) {
		return false;
	}
	size_t count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	unsigned char idLength = targaFileHeader.data1[0];  // Первый байт = id_length
	if (idLength > 0) {
		fseek(filePtr, idLength, SEEK_CUR);  // ← Пропустить ID!
	}

	unsigned char imageType = targaFileHeader.data1[2];  // Третий байт

	if (imageType != 2) {
		return false;
	}

	int bpp;
	m_width = targaFileHeader.width;
	m_height = targaFileHeader.height;
	bpp = static_cast <int> (targaFileHeader.bpp);

	if (bpp != 32) {
		return false;
	}

	imageSize = m_width * m_height * 4;
	targaImage = new unsigned char[imageSize];
	count = static_cast <unsigned int> (fread(targaImage, 1, imageSize, filePtr));
	if (count != imageSize) {
		return false;
	}

	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}

	m_targaData = new unsigned char[imageSize];

	index = 0;

	k = (m_width * m_height * 4) - (m_width * 4);

	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			m_targaData[index + 0] = targaImage[k + 2]; // Red
			m_targaData[index + 1] = targaImage[k + 1]; // Green
			m_targaData[index + 2] = targaImage[k + 0]; // Blue
			m_targaData[index + 3] = targaImage[k + 3]; // Alpha
			k += 4;
			index += 4;
		}
		k -= (m_width * 8);
	}

	delete[] targaImage;
	targaImage = nullptr;

	return true;
}

int TextureClass::GetWidth() {
	return m_width;
}


int TextureClass::GetHeight() {
	return m_height;
}

