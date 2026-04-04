#include "FontClass.hpp"

using namespace DirectX;
using namespace std;

FontClass::FontClass() {
	m_Font = nullptr;
	m_Texture = nullptr;
}

FontClass::FontClass(const FontClass& other) {}

FontClass::~FontClass() {}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int fontChoice) {
    char fontFilename[128];
    char fontTextureFilename[128];
    bool result;

    switch (fontChoice) {
    case 0: {
        strcpy_s(fontFilename, "../DirectXEngine/TestingTextures/font.txt");
        strcpy_s(fontTextureFilename, "../DirectXEngine/TestingTextures/font.tga");
        m_fontHeight = 32.0f;
        m_spaceSize = 3;
        break;
    }
    default: {
        strcpy_s(fontFilename, "../Engine/data/font/font01.txt");
        strcpy_s(fontTextureFilename, "../Engine/data/font/font01.tga");
        m_fontHeight = 32.0f;
        m_spaceSize = 3;
        break;
    }
    }

    result = LoadFontData(fontFilename);
    if (!result) {
        return false;
    }

    result = LoadTexture(device, deviceContext, fontTextureFilename);
    if (!result) {
        return false;
    }

    return true;
}

void FontClass::Shutdown() {
    ReleaseTexture();
    ReleaseFontData();
    return;
}

bool FontClass::LoadFontData(char* filename) {
    ifstream fin;
    int i;
    char temp;

    m_Font = new FontType[95];

    fin.open(filename);
    if (fin.fail()) {
        return false;
    }

    for (i = 0; i < 95; i++) {
        fin.get(temp);
        while (temp != ' ') {
            fin.get(temp);
        }
        fin.get(temp);
        while (temp != ' ') {
            fin.get(temp);
        }

        fin >> m_Font[i].left;
        fin >> m_Font[i].right;
        fin >> m_Font[i].size;
    }

    fin.close();

    return true;
}

void FontClass::ReleaseFontData() {
    if (m_Font) {
        delete[] m_Font;
        m_Font = 0;
    }

    return;
}

bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {
    bool result;


    m_Texture = new TextureClass;

    result = m_Texture->Initialize(device, deviceContext, filename);
    if (!result) {
        return false;
    }

    return true;
}

void FontClass::ReleaseTexture() {
    if (m_Texture) {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = 0;
    }

    return;
}

ID3D11ShaderResourceView* FontClass::GetTexture() {
    return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY) {
    VertexType* vertexPtr;
    int numLetters, index, i, letter;

    vertexPtr = static_cast <VertexType*> (vertices);

    numLetters = static_cast <int> (strlen(sentence));

    index = 0;

    for (i = 0; i < numLetters; i++) {
        letter = (static_cast <int> (sentence[i])) - 32;

        if (letter == 0) {
            drawX = drawX + m_spaceSize;
        }
        else {
            // First triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f);  // Bottom left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
            index++;

            // Second triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            drawX = drawX + m_Font[letter].size + 1.0f;
        }
    }

    return;
}

int FontClass::GetSentencePixelLength(char* sentence) {
    int pixelLength, numLetters, i, letter;


    pixelLength = 0;
    numLetters = static_cast <int> (strlen(sentence));

    for (i = 0; i < numLetters; i++) {
        letter = (static_cast <int> (sentence[i])) - 32;

        if (letter == 0) {
            pixelLength += m_spaceSize;
        }
        else {
            pixelLength += (m_Font[letter].size + 1);
        }
    }

    return pixelLength;
}

int FontClass::GetFontHeight() {
    return static_cast <int> (m_fontHeight);
}