// CAMERA.HPP

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

//#include <d3dx9.h>

class Camera
{
public:
	Camera();

	virtual ~Camera();

	//void Set( D3DXVECTOR3& vPosition, float fHorizontalRotation, float fVerticalRotation );

	//void Rotate( float fHorizontal, float fVertical );

	//void Slide( float fHorizontal, float fVertical );

	//void Move( float fDistance );

	//void GetMatrix( D3DXMATRIXA16& viewMatrix );

private:
	//D3DXMATRIXA16 m_Matrix;

	//D3DXVECTOR3 m_vPosition;
	//D3DXVECTOR3 m_vTarget;
	//D3DXVECTOR3 m_vUpVector;

	float m_fRadius;
	float m_fMoveDistance;

	float m_fHorizontalRadians;
	float m_fVerticalRadians;
};

#endif // _CAMERA_HPP_