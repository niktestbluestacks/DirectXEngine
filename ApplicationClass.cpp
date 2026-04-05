#include "ApplicationClass.hpp"

using namespace DirectX;

ApplicationClass::ApplicationClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_GroundModel = nullptr;
	m_WallModel = nullptr;
	m_BathModel = nullptr;
	m_WaterModel = nullptr;
	m_Light = nullptr;
	m_RefractionTexture = nullptr;
	m_ReflectionTexture = nullptr;
	m_LightShader = nullptr;
	m_RefractionShader = nullptr;
	m_WaterShader = nullptr;
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

	m_Camera->SetPosition(-10.0f, 6.0f, -10.0f);
	m_Camera->SetRotation(0.0f, 45.0f, 0.0f);

	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Ground.txt");

	strcpy_s(textureFilename, "../DirectXEngine/TestingTextures/sprite04.tga");

	m_GroundModel = new ModelClass;

	result = m_GroundModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	// Set the file names of the wall model.
	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Wall.txt");
	strcpy_s(textureFilename, "../directXEngine/TestingTextures/wall01.tga");

	m_WallModel = new ModelClass;

	result = m_WallModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the wall model object.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Bath.txt");
	strcpy_s(textureFilename, "../DirectXEngine/TestingTextures/artem.tga");

	m_BathModel = new ModelClass;

	result = m_BathModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the bath model object.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Water.txt");
	strcpy_s(textureFilename, "../DirectXEngine/TestingTextures/Water01.tga");

	m_WaterModel = new ModelClass;

	result = m_WaterModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the water model object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 0.5f);

	m_RefractionTexture = new RenderTextureClass;

	result = m_RefractionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the refraction render texture object.", L"Error", MB_OK);
		return false;
	}

	m_ReflectionTexture = new RenderTextureClass;

	result = m_ReflectionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the reflection render texture object.", L"Error", MB_OK);
		return false;
	}
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_RefractionShader = new RefractionShaderClass;

	result = m_RefractionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	m_WaterShader = new WaterShaderClass;

	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}
	m_waterHeight = 2.75f;

	m_waterTranslation = 0.0f;

	return true;
}


void ApplicationClass::Shutdown() {
	if (m_WaterShader) {
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = nullptr;
	}

	if (m_RefractionShader) {
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = nullptr;
	}

	if (m_LightShader) {
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

	if (m_ReflectionTexture) {
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = nullptr;
	}

	if (m_RefractionTexture) {
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = nullptr;
	}

	if (m_Light) {
		delete m_Light;
		m_Light = nullptr;
	}

	if (m_WaterModel) {
		m_WaterModel->Shutdown();
		delete m_WaterModel;
		m_WaterModel = nullptr;
	}

	if (m_BathModel) {
		m_BathModel->Shutdown();
		delete m_BathModel;
		m_BathModel = nullptr;
	}

	if (m_WallModel) {
		m_WallModel->Shutdown();
		delete m_WallModel;
		m_WallModel = nullptr;
	}

	if (m_GroundModel) {
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = nullptr;
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
	bool result;


	if (Input->IsEscapePressed()) {
		return false;
	}
	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 1.0f) {
		m_waterTranslation -= 1.0f;
	}

	result = RenderRefractionToTexture();
	if (!result) {
		return false;
	}

	result = RenderReflectionToTexture();
	if (!result) {
		return false;
	}

	result = Render();
	if (!result) {
		return false;
	}

	return true;
}

bool ApplicationClass::RenderRefractionToTexture() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT4 clipPlane;
	bool result;

	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	m_BathModel->Render(m_Direct3D->GetDeviceContext());

	result = m_RefractionShader->Render(m_Direct3D->GetDeviceContext(), m_BathModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_BathModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if (!result) {
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderReflectionToTexture() {
	XMMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
	bool result;


	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->RenderReflection(m_waterHeight);

	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, 6.0f, 8.0f);

	m_WallModel->Render(m_Direct3D->GetDeviceContext());

	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_WallModel->GetIndexCount(),
		worldMatrix, reflectionViewMatrix, projectionMatrix, m_WallModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if (!result) {
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::Render() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;
	bool result;


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);

	m_GroundModel->Render(m_Direct3D->GetDeviceContext());

	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_GroundModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if (!result) {
		return false;
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, 6.0f, 8.0f);

	m_WallModel->Render(m_Direct3D->GetDeviceContext());

	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_WallModel->GetIndexCount(),
			worldMatrix, viewMatrix, projectionMatrix, m_WallModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if (!result) {
		return false;
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	m_BathModel->Render(m_Direct3D->GetDeviceContext());

	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_BathModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_BathModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if (!result) {
		return false;
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);

	m_Camera->GetReflectionViewMatrix(reflectionMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, m_waterHeight, 0.0f);

	m_WaterModel->Render(m_Direct3D->GetDeviceContext());

	result = m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_WaterModel->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix,
		m_ReflectionTexture->GetShaderResourceView(), m_RefractionTexture->GetShaderResourceView(), m_WaterModel->GetTexture(),
		m_waterTranslation, 0.01f);
	if (!result) {
		return false;
	}

	m_Direct3D->EndScene();

	return true;
}