#include "ApplicationClass.hpp"

using namespace DirectX;

ApplicationClass::ApplicationClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_CubeModel = nullptr;
	m_FloorModel = nullptr;
	m_ReflectionShader = nullptr;
	m_TextureShader = nullptr;
	m_RenderTexture = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other) {}

ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	char modelFilename[128], textureFilename[128];
	bool result;

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

	strcpy_s(textureFilename, "../DirectXEngine/TestingTextures/sprite03.tga");

	m_CubeModel = new ModelClass;

	result = m_CubeModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Floor.txt");

	strcpy_s(textureFilename, "../DirectXEngine/TestingTextures/blue01.tga");

	m_FloorModel = new ModelClass;

	result = m_FloorModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}

	m_RenderTexture = new RenderTextureClass;

	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_ReflectionShader = new ReflectionShaderClass;

	result = m_ReflectionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


void ApplicationClass::Shutdown() {
	if (m_ReflectionShader) {
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = nullptr;
	}

	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	if (m_RenderTexture) {
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = nullptr;
	}

	if (m_CubeModel) {
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = nullptr;
	}

	if (m_FloorModel) {
		m_FloorModel->Shutdown();
		delete m_FloorModel;
		m_FloorModel = nullptr;
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

	static float rotation = 0.0f;

	if (Input->IsEscapePressed()) {
		return false;
	}

	rotation -= 0.0174532925f * 0.25f;
	if (rotation < 0.0f) {
		rotation += 360.0f;
	}

	bool result = RenderReflectionToTexture(rotation);
	if (!result) {
		return false;
	}

	result = Render(rotation);
	if (!result) {
		return false;
	}

	return true;
}

bool ApplicationClass::RenderReflectionToTexture(float rotation) {
	XMMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
	bool result;


	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->RenderReflection(-1.5f);

	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation) * XMMatrixRotationX(rotation / 2.0f) * XMMatrixRotationZ(rotation * 3.0f);

	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(),
		worldMatrix, reflectionViewMatrix, projectionMatrix, m_CubeModel->GetTexture());
	if (!result) {
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::Render(float rotation) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix;
	bool result;


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation) * XMMatrixRotationX(rotation / 2.0f) * XMMatrixRotationZ(rotation * 3.0f);

	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_CubeModel->GetTexture());
	if (!result) {
		return false;
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixTranslation(0.0f, -1.5f, 0.0f);

	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	m_FloorModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_FloorModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_FloorModel->GetTexture(), m_RenderTexture->GetShaderResourceView(), reflectionViewMatrix);
	if (!result) {
		return false;
	}

	m_Direct3D->EndScene();

	return true;
}
