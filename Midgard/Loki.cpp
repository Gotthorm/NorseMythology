// LOKI.CPP

//#include <windows.h>
#include "Loki.h"
#include <stdio.h>
#include "Valhalla.h"

const std::wstring shaderName( L"Media/Shaders/render" );

Loki::Loki( Muspelheim::SurfaceID id ) 
	: m_SurfaceID( id )
{
}

bool Loki::Init( Muspelheim::Renderer& renderer )
{
	m_RenderObject = renderer.CreateSurfaceRenderObject( m_SurfaceID );

	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		if( renderObject->SetShader( renderer.LoadShader( shaderName ) ) )
		{
			return true;
		}
	}

	return false;
}

bool Loki::Load( const std::wstring& filename )
{
	if( false == m_RenderObject.expired() )
	{
#if 0
		Valhalla::FBXObject fbxObject;

		if( fbxObject.Load( "Media/Objects/cube.fbx" ) )
		{
			m_RenderObject.lock()->LoadData( fbxObject.GetSize(), fbxObject.GetData(), fbxObject.GetVertices() );

			return true;
		}
#else
		Valhalla::SBMObject sbmObject;

		if( sbmObject.Load( filename ) )
		{
			m_RenderObject.lock()->LoadData( sbmObject.GetSize(), sbmObject.GetData(), sbmObject.GetVertices() );
		
			return true;
		}
#endif
	}

	return false;
}

bool Loki::Update()
{
	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		//static glm::vec3 theRotation;

		//// Perform any updates here to the data buffer
		//renderObject->SetRotation( theRotation );

		//theRotation.y += 0.0001f;

		// Call the deferred render
		return renderObject->Render();
	}

	return false;
}

void Loki::SetPosition( const glm::vec3& position )
{
	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		renderObject->SetPosition( position );
	}
}
