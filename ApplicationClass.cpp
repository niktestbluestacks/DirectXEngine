#include "ApplicationClass.hpp"
#include "MyConverter.hpp"

using namespace DirectX;

ApplicationClass::ApplicationClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_FontShader = nullptr;
	m_Font = nullptr;
	m_TextString1 = nullptr;
	m_TextString2 = nullptr;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other) {}


ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;
	char testString1[32], testString2[32];
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();


	m_FontShader = new FontShaderClass;

	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	m_Font = new FontClass;

	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result) {
		return false;
	}

	strcpy_s(testString1, "Artem");
	strcpy_s(testString2, "Pidor");

	m_TextString1 = new TextClass;

	result = m_TextString1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, m_Font, testString1, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result) {
		return false;
	}

	// Create and initialize the second text object.
	m_TextString2 = new TextClass;

	result = m_TextString2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, m_Font, testString2, 10, 50, 1.0f, 1.0f, 0.0f);
	if (!result) {
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown() {
	if (m_TextString2) {
		m_TextString2->Shutdown();
		delete m_TextString2;
		m_TextString2 = nullptr;
	}

	if (m_TextString1) {
		m_TextString1->Shutdown();
		delete m_TextString1;
		m_TextString1 = nullptr;
	}

	if (m_Font) {
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}

	if (m_FontShader) {
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	if (m_Camera) {
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	return;
}

bool ApplicationClass::Frame() {
	bool result;

	result = Render();

	if (!result) {
		return false;
	}
	return true;
}

bool ApplicationClass::Render() {
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();

	m_TextString1->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString1->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString1->GetPixelColor());
	if (!result) {
		return false;
	}

	m_TextString2->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_TextString2->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_TextString2->GetPixelColor());
	if (!result) {
		return false;
	}

	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	m_Direct3D->EndScene();

	return true;
}


