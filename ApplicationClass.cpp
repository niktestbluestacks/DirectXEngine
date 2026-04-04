#include "ApplicationClass.hpp"

using namespace DirectX;

ApplicationClass::ApplicationClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_Light = nullptr;
	m_ShaderManager = nullptr;
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
	m_Camera->SetPosition(0.0f, 0.0f, -8.0f);
	m_Camera->Render();

	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Sphere.txt");

	strcpy_s(textureFilename1, "../DirectXEngine/TestingTextures/sprite03.tga");
	strcpy_s(textureFilename2, "../DirectXEngine/TestingTextures/normal02.tga");

	m_Model = new ModelClass;
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename1, textureFilename2);
	if (!result) {
		return false;
	}

	m_Light = new LightClass;

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	m_ShaderManager = new ShaderManagerClass;
	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown() {

	if (m_ShaderManager) {
		delete m_ShaderManager;
		m_ShaderManager = nullptr;
	}

	if (m_Light) {
		delete m_Light;
		m_Light = nullptr;
	}

	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
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
	static float rotation = 360.0f;
	bool result;


	if (Input->IsEscapePressed()) {
		return false;
	}

	rotation -= 0.0174532925f * 0.25f;
	if (rotation <= 0.0f) {
		rotation += 360.0f;
	}

	result = Render(rotation);
	if (!result) {
		return false;
	}

	return true;
}

bool ApplicationClass::Render(float rotation) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotateMatrix, translateMatrix;
	bool result;


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	rotateMatrix = XMMatrixRotationY(rotation) * XMMatrixRotationX(rotation / 2.0f) * XMMatrixRotationY(rotation * 3.0f);
	translateMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_ShaderManager->RenderTextureShader(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(0));
	if (!result) {
		return false;
	}

	rotateMatrix = XMMatrixRotationY(rotation) * XMMatrixRotationX(rotation / 2.0f) * XMMatrixRotationY(rotation * 3.0f);
	translateMatrix = XMMatrixTranslation(-1.5f, -1.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_ShaderManager->RenderLightShader(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(0), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result) {
		return false;
	}

	rotateMatrix = XMMatrixRotationY(rotation) * XMMatrixRotationX(rotation / 2.0f) * XMMatrixRotationY(rotation * 3.0f);
	translateMatrix = XMMatrixTranslation(1.5f, -1.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_ShaderManager->RenderNormalMapShader(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(0), m_Model->GetTexture(1), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result) {
		return false;
	}

	m_Direct3D->EndScene();

	return true;
}
