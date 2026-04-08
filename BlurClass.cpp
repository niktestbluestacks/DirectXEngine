#include "BlurClass.hpp"

using namespace DirectX;

BlurClass::BlurClass() {
    m_DownSampleTexure1 = nullptr;
    m_DownSampleTexure2 = nullptr;
    m_DownSampleWindow = nullptr;
    m_UpSampleWindow = nullptr;
}

BlurClass::BlurClass(const BlurClass& other) {}

BlurClass::~BlurClass() {}

bool BlurClass::Initialize(D3DClass* Direct3D, int downSampleWidth, int downSampleHeight,
    float screenNear, float screenDepth, int renderWidth, int renderHeight) {

    bool result;


    m_downSampleWidth = downSampleWidth;
    m_downSampleHeight = downSampleHeight;

    m_DownSampleTexure1 = new RenderTextureClass;

    result = m_DownSampleTexure1->Initialize(Direct3D->GetDevice(), m_downSampleWidth, m_downSampleHeight,
        screenDepth, screenNear, 1);
    if (!result) {
        return false;
    }

    m_DownSampleTexure2 = new RenderTextureClass;

    result = m_DownSampleTexure2->Initialize(Direct3D->GetDevice(), m_downSampleWidth, m_downSampleHeight,
        screenDepth, screenNear, 1);
    if (!result) {
        return false;
    }

    m_DownSampleWindow = new OrthoWindowClass;

    result = m_DownSampleWindow->Initialize(Direct3D->GetDevice(), m_downSampleWidth, m_downSampleHeight);
    if (!result) {
        return false;
    }

    m_UpSampleWindow = new OrthoWindowClass;

    result = m_UpSampleWindow->Initialize(Direct3D->GetDevice(), renderWidth, renderHeight);
    if (!result) {
        return false;
    }

    return true;
}

void BlurClass::Shutdown() {
    if (m_UpSampleWindow) {
        m_UpSampleWindow->Shutdown();
        delete m_UpSampleWindow;
        m_UpSampleWindow = nullptr;
    }

    if (m_DownSampleWindow) {
        m_DownSampleWindow->Shutdown();
        delete m_DownSampleWindow;
        m_DownSampleWindow = nullptr;
    }

    if (m_DownSampleTexure2) {
        m_DownSampleTexure2->Shutdown();
        delete m_DownSampleTexure2;
        m_DownSampleTexure2 = nullptr;
    }

    if (m_DownSampleTexure1) {
        m_DownSampleTexure1->Shutdown();
        delete m_DownSampleTexure1;
        m_DownSampleTexure1 = nullptr;
    }

    return;
}


bool BlurClass::BlurTexture(D3DClass* Direct3D, CameraClass* Camera,
    RenderTextureClass* RenderTexture, TextureShaderClass* TextureShader, BlurShaderClass* BlurShader) {

    XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
    float blurType;
    bool result;

    Direct3D->GetWorldMatrix(worldMatrix);
    Camera->GetViewMatrix(viewMatrix);

    Direct3D->TurnZBufferOff();

    // STEP 1: Down sample the render to texture.

    m_DownSampleTexure1->SetRenderTarget(Direct3D->GetDeviceContext());
    m_DownSampleTexure1->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    m_DownSampleTexure1->GetOrthoMatrix(orthoMatrix);

    m_DownSampleWindow->Render(Direct3D->GetDeviceContext());

    result = TextureShader->Render(Direct3D->GetDeviceContext(), m_DownSampleWindow->GetIndexCount(),
        worldMatrix, viewMatrix, orthoMatrix, RenderTexture->GetShaderResourceView());
    if (!result) {
        return false;
    }

    // STEP 2: Perform a horizontal blur on the down sampled texture.

    // Set the blur type to zero for a horizontal blur from the blur shader.
    blurType = 0.0f;

    m_DownSampleTexure2->SetRenderTarget(Direct3D->GetDeviceContext());
    m_DownSampleTexure2->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    m_DownSampleTexure2->GetOrthoMatrix(orthoMatrix);

    m_DownSampleWindow->Render(Direct3D->GetDeviceContext());

    result = BlurShader->Render(Direct3D->GetDeviceContext(), m_DownSampleWindow->GetIndexCount(),
        worldMatrix, viewMatrix, orthoMatrix, m_DownSampleTexure1->GetShaderResourceView(),
        m_downSampleWidth, m_downSampleHeight, blurType);
    if (!result) {
        return false;
    }

    // STEP 3: Perform a vertical blur on the horizontally blurred texture.

    // Set the blur type to one for a vertical blur from the blur shader.
    blurType = 1.0f;

    m_DownSampleTexure1->SetRenderTarget(Direct3D->GetDeviceContext());
    m_DownSampleTexure1->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    m_DownSampleTexure1->GetOrthoMatrix(orthoMatrix);

    m_DownSampleWindow->Render(Direct3D->GetDeviceContext());

    result = BlurShader->Render(Direct3D->GetDeviceContext(), m_DownSampleWindow->GetIndexCount(),
        worldMatrix, viewMatrix, orthoMatrix, m_DownSampleTexure2->GetShaderResourceView(),
        m_downSampleWidth, m_downSampleHeight, blurType);
    if (!result) {
        return false;
    }

    // STEP 4: Up sample the blurred result.

    RenderTexture->SetRenderTarget(Direct3D->GetDeviceContext());
    RenderTexture->ClearRenderTarget(Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
    RenderTexture->GetOrthoMatrix(orthoMatrix);

    m_UpSampleWindow->Render(Direct3D->GetDeviceContext());

    result = TextureShader->Render(Direct3D->GetDeviceContext(), m_UpSampleWindow->GetIndexCount(),
        worldMatrix, viewMatrix, orthoMatrix, m_DownSampleTexure1->GetShaderResourceView());
    if (!result) {
        return false;
    }

    Direct3D->TurnZBufferOn();

    Direct3D->SetBackBufferRenderTarget();
    Direct3D->ResetViewport();

    return true;
}