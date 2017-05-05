// LOKI.CPP

#include <windows.h>
#include "Loki.h"
#include <stdio.h>
#include "Valhalla.h"

const std::wstring shaderName( L"Media/Shaders/render" );

Loki::Loki( Muspelheim::SurfaceID id ) : m_SurfaceID( id )
{
}

Loki::~Loki()
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

bool Loki::Load( const std::string& filename )
{
	if( m_RenderObject.expired() == false )
	{
		Valhalla::SBMObject sbmObject;

		Valhalla::FBXObject fbxObject;

		if( fbxObject.Load( "Media/Objects/cube.fbx" ) )
		{
			m_RenderObject.lock()->LoadData( fbxObject.GetSize(), fbxObject.GetData(), fbxObject.GetVertices() );

			return true;
		}

		if( sbmObject.Load( filename ) )
		{
			m_RenderObject.lock()->LoadData( sbmObject.GetSize(), sbmObject.GetData(), sbmObject.GetVertices() );
		
			return true;
		}
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

void Loki::SetPosition( const glm::vec4& position )
{
	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		renderObject->SetPosition( position );
	}
}