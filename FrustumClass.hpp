#pragma once
#ifndef _FRUSTUMCLASS_HPP_
#define _FRUSTUMCLASS_HPP_

// Includes
#include <DirectXMath.h>

// Class name: FrustumClass
class FrustumClass {
public:
    FrustumClass();
    FrustumClass(const FrustumClass&);
    ~FrustumClass();

    void ConstructFrustum(DirectX::XMMATRIX, DirectX::XMMATRIX, float);

    bool CheckPoint(float, float, float);
    bool CheckCube(float, float, float, float);
    bool CheckSphere(float, float, float, float);
    bool CheckRectangle(float, float, float, float, float, float);

private:
    DirectX::XMFLOAT4 m_planes[6];
};
#endif

