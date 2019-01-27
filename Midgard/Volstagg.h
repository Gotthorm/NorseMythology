// VOLSTAGG.H

#pragma once

#include <string>
#include <memory>
#include "Muspelheim.h"

class Volstagg
{
public:
	Volstagg( Muspelheim::SurfaceID id );
    virtual ~Volstagg() = default;

	bool Init( Muspelheim::Renderer& renderer );

    bool Load( const std::wstring& fileName );

	bool Update();

private:
	std::weak_ptr<Muspelheim::RenderObject> m_RenderObject;
	Muspelheim::SurfaceID m_SurfaceID;
	//unsigned int m_DisplacementTexture = 0;
};
