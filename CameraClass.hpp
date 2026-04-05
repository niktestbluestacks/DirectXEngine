#pragma once
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

// Includes
#include <DirectXMath.h>

// Class name: CameraClass
class CameraClass {
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

	void RenderReflection(float);
	void GetReflectionViewMatrix(DirectX::XMMATRIX&);
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_reflectionViewMatrix;
};
#endif // _CAMERACLASS_H_

