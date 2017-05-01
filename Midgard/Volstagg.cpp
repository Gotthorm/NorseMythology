// VOLSTAGG.CPP

#include <windows.h>
#include "Volstagg.h"
#include "Valhalla.h"
#include <stdio.h>

const std::wstring shaderName( L"Media/Shaders/tessellation" );

Volstagg::Volstagg( Muspelheim::SurfaceID id ) : m_SurfaceID( id )
{
}

Volstagg::~Volstagg()
{
}

bool Volstagg::Init( Muspelheim::Renderer& renderer )
{
	m_RenderObject = renderer.CreateSurfaceRenderObject( m_SurfaceID );

	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		if( renderObject->SetShader( renderer.LoadShader( shaderName ) ) )
		{
			// Need to figure out why this is necessary, rotating the mesh by 90 degrees
			renderObject->Pitch( 1.5708f );

			return true;
		}
	}

	return false;
}

bool Volstagg::Load( const std::string& displacementTextureName )
{
	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		Valhalla::PNGObject png;

		if( png.Load( displacementTextureName ) )
		{
			renderObject->LoadTexture( png.GetData(), png.GetWidth(), png.GetHeight(), png.GetAlpha() );
		}

		if( png.Load( "Media/Textures/Green.png" ) )
		{
			renderObject->LoadTexture( png.GetData(), png.GetWidth(), png.GetHeight(), png.GetAlpha() );
		}

		renderObject->SetWindingOrderClockwise( true );
		renderObject->SetPolyLineMode( true );
		renderObject->SetPolyBackFace( true );
		renderObject->SetPolyMode( Muspelheim::RenderObject::PATCH );

		return true;
	}
	
	return false;
}

bool Volstagg::Update()
{
	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( renderObject != nullptr )
	{
		// Perform any updates here to the data buffer

		// Call the deferred render
		return renderObject->Render();
	}

	return false;
}