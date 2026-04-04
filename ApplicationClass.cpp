#include "ApplicationClass.hpp"
#include "MyConverter.hpp"

using namespace DirectX;

ApplicationClass::ApplicationClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_LightShader = nullptr;
	m_Light = nullptr;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other) {}


ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;
	char textureFilename[128];
	char modelFilename[128];
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	m_Camera = new CameraClass;

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_Model = new ModelClass;

	auto textureStr = MyConverter::constCharPtrPathToString("../DirectXEngine/TestingTextures/stone01.tga");
	auto textureCStr = textureStr->c_str();

	auto modelStr = MyConverter::constCharPtrPathToString("../DirectXEngine/TestingTextures/Sphere.txt");
	auto modelCStr = modelStr->c_str();

	strcpy_s(textureFilename, 128, textureCStr);
	strcpy_s(modelFilename, 128, modelCStr);
	textureStr.reset();
	modelStr.reset();
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void ApplicationClass::Shutdown() {
	if (m_Light) {
		delete m_Light;
		m_Light = nullptr;
	}
	
	if (m_LightShader) {
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
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

bool ApplicationClass::Frame() {
	static float rotation = 0.0f;
	bool result;

	rotation -= 0.0174532925f * 0.25f;

	if (rotation < 0.0f) {
		rotation += 360.0f;
	}

	result = Render(rotation);
	if (!result) {
		return false;
	}
	return true;
}

bool ApplicationClass::Render(float rotation) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	bool result;


	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	rotateMatrix = XMMatrixRotationY(rotation)/* * XMMatrixRotationX(rotation / 2.0f) * XMMatrixRotationZ(rotation * 3.0f)*/;
	translateMatrix = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	/*if (!result) {
		return false;
	}

	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);  
	rotateMatrix = XMMatrixRotationY(rotation)// * XMMatrixRotationX(rotation / -2.0f) * XMMatrixRotationZ(rotation * 3.0f);
	translateMatrix = XMMatrixTranslation(2.0f, 0.0f, 0.0f);  

	// Multiply the scale, rotation, and translation matrices together to create the final world transformation matrix.
	srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());*/

	if (!result) {
		return false;
	}

	m_Direct3D->EndScene();
	return true;
}


