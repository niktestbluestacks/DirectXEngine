#include "ApplicationClass.hpp"

using namespace DirectX;
using namespace std;

ApplicationClass::ApplicationClass() {
    m_Direct3D = nullptr;
    m_Camera = nullptr;
    m_Model = nullptr;
	m_DepthShader = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other) {}

ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
    char modelFilename[128], textureFilename1[128], textureFilename2[128];
    bool result;


    m_Direct3D = new D3DClass;

    result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
        return false;
    }

    m_Camera = new CameraClass;

    m_Camera->SetPosition(0.0f, 2.0f, -10.0f);
    m_Camera->Render();

    strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Floor.txt");

    m_Model = new ModelClass;

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    m_DepthShader = new DepthShaderClass;

    result = m_DepthShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}


void ApplicationClass::Shutdown() {
    if (m_DepthShader) {
        m_DepthShader->Shutdown();
        delete m_DepthShader;
        m_DepthShader = nullptr;
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
    static float rotation = 0.0f;
    bool result;


    if (Input->IsEscapePressed()) {
        return false;
    }

    rotation -= 0.0174532925f * 0.25f;
    if (rotation < 0.0f) {
        rotation += 360.0f;
    }

    /*result = RenderSceneToTexture(rotation);
    if (!result) {
        return false;
    }

    result = Render(rotation);
    if (!result) {
        return false;
    }*/

    result = Render();
    if (!result) {
        return false;
    }

    return true;
}

/*bool ApplicationClass::RenderSceneToTexture(float rotation) {
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;


    m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
    m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    worldMatrix = XMMatrixRotationY(rotation);

    m_Model->Render(m_Direct3D->GetDeviceContext());

    result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(0));
    if (!result) {
        return false;
    }

    m_Direct3D->SetBackBufferRenderTarget();
    m_Direct3D->ResetViewport();

    return true;
}


bool ApplicationClass::Render(float rotation) {
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    float refractionScale;
    bool result;

    refractionScale = 0.01f;

    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    worldMatrix = XMMatrixRotationY(rotation);

    m_Model->Render(m_Direct3D->GetDeviceContext());

    result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(0));
    if (!result) {
        return false;
    }

    worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -1.5f);

    m_WindowModel->Render(m_Direct3D->GetDeviceContext());

    result = m_GlassShader->Render(m_Direct3D->GetDeviceContext(), m_WindowModel->GetIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix, m_WindowModel->GetTexture(0),
        m_WindowModel->GetTexture(1), m_RenderTexture->GetShaderResourceView(), refractionScale);
    if (!result) {
        return false;
    }

    m_Direct3D->EndScene();

    return true;
}*/

bool ApplicationClass::Render() {
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;


    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    m_Model->Render(m_Direct3D->GetDeviceContext());

    result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix);
    if (!result) {
        return false;
    }

    m_Direct3D->EndScene();

    return true;
}