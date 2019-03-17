#pragma once

#ifdef MAKE_DLL  
#define EXPORT __declspec(dllexport)   
#else  
#define EXPORT __declspec(dllimport)   
#endif  

#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Helheimr.h"

namespace Vanaheimr
{
	const float pi = 3.14159265358979323846f;
	constexpr float Deg2Rad( float value ) { return value * (pi / 180.0f); }
	constexpr float Rad2Deg( float value ) { return value * (180.0f / pi); }

	class EXPORT Object3D
	{
	public:
		Object3D() = default;
		virtual ~Object3D() = default;

		const std::wstring& GetName() const { return m_Name; }
		void SetName( const std::wstring& name ) { m_Name = name; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition( const glm::vec3& position ) { m_Position = position; }

		const glm::quat& GetOrientation() const { return m_Orientation; }
		//void SetOrientation( const glm::quat& orientation ) { m_Orientation = orientation; }

		virtual glm::mat4 GetMatrix() const { return glm::translate( glm::mat4_cast( m_Orientation ), m_Position ); }

		void Rotate( float angle, const glm::vec3 &axis ) { m_Orientation *= glm::angleAxis( angle, axis * m_Orientation ); }
		void Rotate( float angle, float x, float y, float z ) { m_Orientation *= glm::angleAxis( angle, glm::vec3( x, y, z ) * m_Orientation ); }

		void Translate( const glm::vec3 &v ) { m_Position += v * m_Orientation; }
		void Translate( float x, float y, float z ) { m_Position += glm::vec3( x, y, z ) * m_Orientation; }

		// Helpers
		void Yaw( float angle ) { Rotate( angle, 0.0f, 1.0f, 0.0f ); }
		void Pitch( float angle ) { Rotate( angle, 1.0f, 0.0f, 0.0f ); }
		void Roll( float angle ) { Rotate( angle, 0.0f, 0.0f, 1.0f ); }

	private:
		// These private attributes do not need a DLL interface declaration
#pragma warning( push )
#pragma warning( disable : 4251)
		glm::vec3 m_Position;
		glm::quat m_Orientation;
		std::wstring m_Name;
#pragma warning( pop ) 
	};

	class EXPORT Camera : public Object3D
	{
	public:
		//virtual void Update( float timeElapsed ) = 0;
	};

	class EXPORT CameraManager
	{
	public:
		enum Mode
		{
			FIXED,
			FOLLOW,
			ORBIT,
			FREE,
		};

		typedef unsigned int CameraId;

		CameraManager();
		virtual ~CameraManager();

		void Update( float timeElapsed, std::shared_ptr<Helheimr::Input> const & input );

		// Create a new camera and add to the list
		// Returns an unique identifier ID for the camera
		// Returns 0 if call fails
		CameraId CameraManager::AddCamera( Mode mode, const std::wstring& name );

		// No assumptions made on the given camera index, GetCamera will handle any incorrect input
		const Camera* GetCurrentCamera() { return GetCamera( m_CurrentCameraIndex ); }

		bool SetCurrentCamera( CameraId cameraId );

		bool SetCurrentCameraToNext();

		// If the given camera index is valid, return the corresponding camera, otherwise return null
		const Camera* GetCamera( CameraId cameraIndex );

		bool SetCameraTarget( CameraId cameraId, Object3D* target );

		bool SetCameraPosition( CameraId cameraId, const glm::vec3& position );

		bool SetCameraOffset( CameraId cameraId, const glm::vec3& position );

	private:
		CameraId m_CurrentCameraIndex;

		// These private attributes do not need a DLL interface declaration
#pragma warning( push )
#pragma warning( disable : 4251)
		std::vector<std::pair<Camera*, Mode>> m_Cameras;
#pragma warning( pop ) 
	};

	template <class T>
	class EXPORT RingBuffer
	{
	public:
		explicit RingBuffer( size_t size ) : m_MaxSize( size ), m_Buffer( size )
		{
		}

		void Push( T item )
		{
			if ( m_Buffer.size() <= m_HeadIndex )
			{
				m_Buffer.push_back( item );
			}
			else
			{
				m_Buffer[ m_HeadIndex ] = item;
			}
			
			if ( m_Full ) m_TailIndex = ( m_TailIndex + 1 ) % m_MaxSize;
			m_HeadIndex = ( m_HeadIndex + 1 ) % m_MaxSize;
			m_Full = m_HeadIndex == m_TailIndex;
		}

		T Pop()
		{
			if ( Empty() ) return T();
			T item = m_Buffer[ m_TailIndex ];
			m_Full = false;
			m_TailIndex = ( m_TailIndex + 1 ) % m_MaxSize;
			return item;
		}

		void Reset() { m_HeadIndex = m_TailIndex; m_Full = false; }

		bool Empty() const { return ( !m_Full && ( m_HeadIndex == m_TailIndex ) ); }

		bool Full() const { return m_Full; }

		size_t Capacity() const { return m_MaxSize; }

		size_t Size() const
		{
			size_t size = m_MaxSize;
			if ( !m_Full )
			{
				if ( m_HeadIndex >= m_TailIndex )
				{
					size = m_HeadIndex - m_TailIndex;
				}
				else
				{
					size = m_MaxSize + m_HeadIndex - m_TailIndex;
				}
			}
			return size;
		}

		void Resize( size_t size )
		{
			while ( Size() > size ) Pop();
			std::vector<T> tempBuffer( Size() );
			while ( Size() > 0 ) tempBuffer.push_back( Pop() );
			m_TailIndex = 0;
			m_HeadIndex = tempBuffer.size();
			m_Buffer.swap( tempBuffer );
			m_MaxSize = size;
		}

		T & operator[] ( int index )
		{
			if ( index >= Size() ) throw;
			return m_Buffer[ ( m_TailIndex + index ) % m_MaxSize ];
		}

	private:
		std::vector<T> m_Buffer;
		size_t m_HeadIndex = 0;
		size_t m_TailIndex = 0;
		size_t m_MaxSize;
		bool m_Full = false;
	};
}


