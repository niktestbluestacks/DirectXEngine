#include "ApplicationClass.hpp"

using namespace DirectX;
using namespace std;

ApplicationClass::ApplicationClass() {
    m_Direct3D = nullptr;
    m_Camera = nullptr;
    m_Model = nullptr;
    m_TextureShader = nullptr;
    m_RenderTexture = nullptr;
    m_FullScreenWindow = nullptr;
    m_FadeShader = nullptr;
    m_Timer = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other) {}

ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
    char modelFilename[128], textureFilename1[128];
    int downSampleWidth, downSampleHeight;
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

    m_Model = new ModelClass;

    strcpy_s(modelFilename, "../DirectXEngine/TestingTextures/Cube.txt");
    strcpy_s(textureFilename1, "../DirectXEngine/TestingTextures/sprite03.tga");

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
        modelFilename, textureFilename1);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    m_TextureShader = new TextureShaderClass;

    result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
        return false;
    }

    m_RenderTexture = new RenderTextureClass;

    result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(),
        screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, 0);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
        return false;
    }

    m_FullScreenWindow = new OrthoWindowClass;

    result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
        return false;
    }

    downSampleWidth = screenWidth / 2;
    downSampleHeight = screenHeight / 2;

    m_FadeShader = new FadeShaderClass;

    result = m_FadeShader->Initialize(m_Direct3D->GetDevice(), hwnd);
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the fade shader object.", L"Error", MB_OK);
        return false;
    }

    m_Timer = new TimerClass;

    result = m_Timer->Initialize();
    if (!result) {
        MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
        return false;
    }

    m_accumulatedTime = 0.0f;

    m_fadeInTime = 5.0f;

    return true;
}

void ApplicationClass::Shutdown() {
    if (m_Timer) {
        delete m_Timer;
        m_Timer = nullptr;
    }

    if (m_FadeShader) {
        m_FadeShader->Shutdown();
        delete m_FadeShader;
        m_FadeShader = nullptr;
    }

    if (m_FullScreenWindow) {
        m_FullScreenWindow->Shutdown();
        delete m_FullScreenWindow;
        m_FullScreenWindow = nullptr;
    }

    if (m_RenderTexture) {
        m_RenderTexture->Shutdown();
        delete m_RenderTexture;
        m_RenderTexture = nullptr;
    }

    if (m_TextureShader) {
        m_TextureShader->Shutdown();
        delete m_TextureShader;
        m_TextureShader = nullptr;
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
    static bool FadeIn = true;
    float frameTime;
    float fadePercentage;
    bool result;

    m_Timer->Frame();

    if (Input->IsEscapePressed()) {
        return false;
    }

    rotation -= 0.0174532925f * 0.25f;
    if (rotation < 0.0f) {
        rotation += 360.0f;
    }

    frameTime = m_Timer->GetTime();
    if (FadeIn) {
        m_accumulatedTime += frameTime;

        if (m_accumulatedTime < m_fadeInTime) {
            fadePercentage = m_accumulatedTime / m_fadeInTime;
        }
        else {
            fadePercentage = 1.0f;
            FadeIn = false;
        }
    } 
    else {
        m_accumulatedTime -= frameTime;

        if (m_accumulatedTime > 0) {
            fadePercentage = m_accumulatedTime / m_fadeInTime;
        }
        else {
            fadePercentage = 0.0f;
            FadeIn = true;
        }
    }

    result = RenderSceneToTexture(rotation);
    if (!result) {
        return false;
    }

    result = Render(fadePercentage);
    if (!result) {
        return false;
    }

    return true;
}

bool ApplicationClass::RenderSceneToTexture(float rotation) {
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;


    m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
    m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_RenderTexture->GetProjectionMatrix(projectionMatrix);

    worldMatrix = XMMatrixRotationY(rotation);

    m_Model->Render(m_Direct3D->GetDeviceContext());

    result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
    if (!result) {
        return false;
    }

    m_Direct3D->SetBackBufferRenderTarget();
    m_Direct3D->ResetViewport();

    return true;
}

bool ApplicationClass::Render(float fadePercentage) {
    XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
    bool result;


    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);

    m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

    result = m_FadeShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
        worldMatrix, viewMatrix, orthoMatrix,
        m_RenderTexture->GetShaderResourceView(), fadePercentage);
    if (!result) {
        return false;
    }

    m_Direct3D->EndScene();

    return true;
}

/*bool ApplicationClass::Render() {
    XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
    bool result;


    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);

    m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

    result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
        worldMatrix, viewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView());
    if (!result) {
        return false;
    }

    m_Direct3D->EndScene();

    return true;
}*/


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