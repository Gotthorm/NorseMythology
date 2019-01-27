// VOLSTAGG.CPP

//#include <windows.h>
#include "Volstagg.h"
#include "Valhalla.h"
#include <stdio.h>

std::wstring const shaderName( L"Media/Shaders/tessellation" );

Volstagg::Volstagg( Muspelheim::SurfaceID id ) 
	: m_SurfaceID( id )
{
}

bool Volstagg::Init( Muspelheim::Renderer& renderer )
{
	m_RenderObject = renderer.CreateSurfaceRenderObject( m_SurfaceID );

	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( nullptr != renderObject )
	{
		if( renderObject->SetShader( renderer.LoadShader( shaderName ) ) )
		{
			// Need to figure out why this is necessary
			renderObject->Pitch( Vanaheimr::Deg2Rad(90) );

			return true;
		}
	}

	return false;
}

bool Volstagg::Load( const std::wstring& displacementTextureName )
{
	std::shared_ptr<Muspelheim::RenderObject> renderObject = m_RenderObject.lock();

	if( nullptr != renderObject )
	{
		Valhalla::PNGObject png;

		if( png.Load( displacementTextureName ) )
		{
			renderObject->LoadTexture( png.GetData(), png.GetWidth(), png.GetHeight(), png.GetAlpha() );
		}

		if( png.Load( L"Media/Textures/Green.png" ) )
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