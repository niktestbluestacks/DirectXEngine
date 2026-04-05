#include "ApplicationClass.hpp"

using namespace DirectX;

ApplicationClass::ApplicationClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model1 = nullptr;
	m_Model2 = nullptr;
	m_TextureShader = nullptr;
	m_TransparentShader = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other) {}

ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;
	char modelFilename[128], textureFilename1[128], textureFilename2[128];
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();

	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Cube.txt");

	strcpy_s(textureFilename1, "../DirectXEngine/TestingTextures/sprite03.tga");
	strcpy_s(textureFilename2, "../DirectXEngine/TestingTextures/sprite04.tga");

	m_Model1 = new ModelClass;
	result = m_Model1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename1);
	if (!result) {
		return false;
	}

	m_Model2 = new ModelClass;

	result = m_Model2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename2);
	if (!result) {
		return false;
	}

	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_TransparentShader = new TransparentShaderClass;

	result = m_TransparentShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown() {
	if (m_TransparentShader) {
		m_TransparentShader->Shutdown();
		delete m_TransparentShader;
		m_TransparentShader = nullptr;
	}

	if (m_Model1) {
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = nullptr;
	}

	if (m_Model2) {
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = nullptr;
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

bool ApplicationClass::Frame(InputClass* Input) {

	
	if (Input->IsEscapePressed()) {
		return false;
	}

	bool result = Render();
	if (!result) {
		return false;
	}

	return true;
}

bool ApplicationClass::Render() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	float blendAmount;
	bool result;

	blendAmount = 0.5f;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Model1->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model1->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model1->GetTexture());
	if (!result) {
		return false;
	}

	worldMatrix = XMMatrixTranslation(1.0f, 0.0f, -1.0f);

	m_Direct3D->EnableAlphaBlending();

	m_Model2->Render(m_Direct3D->GetDeviceContext());

	result = m_TransparentShader->Render(m_Direct3D->GetDeviceContext(), m_Model2->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model2->GetTexture(), blendAmount);
	if (!result) {
		return false;
	}

	// Turn off alpha blending.
	m_Direct3D->DisableAlphaBlending();

	m_Direct3D->EndScene();

	return true;
}