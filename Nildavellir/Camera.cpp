// CAMERA.CPP

#include "Camera.h"
#include <stdio.h>

Camera::Camera() : m_fRadius( 1.0f ), m_fMoveDistance( 1.0f )
{
}

Camera::~Camera()
{
}

//void Camera::Set( D3DXVECTOR3& vPosition, float fHorizontalRotation, float fVerticalRotation )
//{
//    m_vPosition = vPosition;
//    m_vTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//    m_vUpVector = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//    m_fHorizontalRadians = fHorizontalRotation;
//    m_fVerticalRadians = fVerticalRotation;
//
//    Rotate( 0.0f, 0.0f );
//}
//
//void Camera::Rotate( float fHorizontal, float fVertical )
//{
//    m_fHorizontalRadians += fHorizontal;
//    m_fVerticalRadians += fVertical;
//
//	TCHAR theString[ 256 ];
//	sprintf( theString, "Horizontal( %f )  Vertical( %f )\n", m_fHorizontalRadians, m_fVerticalRadians );
//	OutputDebugString( theString );
//
//    m_vTarget.y = m_vPosition.y + ( m_fRadius * sin( m_fVerticalRadians ) );
//    m_vTarget.x = m_vPosition.x + ( m_fRadius * cos( m_fVerticalRadians ) * cos( m_fHorizontalRadians ) );
//    m_vTarget.z = m_vPosition.z + ( m_fRadius * cos( m_fVerticalRadians ) * sin( m_fHorizontalRadians ) );
//    
//    m_vUpVector.x = m_vPosition.x - m_vTarget.x;
//    m_vUpVector.y = abs( m_vPosition.y + ( m_fRadius * sin( m_fVerticalRadians + D3DX_PI / 2.0f ) ) ) ;
//    m_vUpVector.z = m_vPosition.z - m_vTarget.z;
//
//	D3DXMatrixLookAtLH( &m_Matrix, &m_vPosition, &m_vTarget, &m_vUpVector );
//}
//
//void Camera::Slide( float fHorizontal, float fVertical )
//{
//    m_vPosition.y += fVertical * m_fMoveDistance;
//    m_vPosition.x += fHorizontal * m_fMoveDistance * cos( m_fHorizontalRadians + D3DX_PI / 2.0f );
//    m_vPosition.z += fHorizontal * m_fMoveDistance * sin( m_fHorizontalRadians + D3DX_PI / 2.0f );
//    Rotate( 0.0f, 0.0f );
//}
//
//void Camera::Move( float fDistance )
//{
//    m_vPosition.y += fDistance * m_fMoveDistance * sin( m_fVerticalRadians );
//    m_vPosition.x += fDistance * m_fMoveDistance * ( cos( m_fVerticalRadians ) * cos( m_fHorizontalRadians ) );
//    m_vPosition.z += fDistance * m_fMoveDistance * ( cos( m_fVerticalRadians ) * sin( m_fHorizontalRadians ) );
//    Rotate( 0.0f, 0.0f );
//}
//
//void Camera::GetMatrix( D3DXMATRIXA16& viewMatrix )
//{
//	viewMatrix = m_Matrix;
//}
