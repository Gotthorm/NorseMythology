// LOKI.H

#pragma once

//#include "VectorMath.h"
#include <string>
#include <memory>
#include "Muspelheim.h"

class Loki
{
public:
	Loki( Muspelheim::SurfaceID id );
	virtual ~Loki() = default;

	bool Init( Muspelheim::Renderer& renderer );

    bool Load( const std::wstring& filename );

	bool Update();

	void SetPosition( const glm::vec3& position );

private:
	std::weak_ptr<Muspelheim::RenderObject> m_RenderObject;
	Muspelheim::SurfaceID m_SurfaceID;
};
