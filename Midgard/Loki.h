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

	bool Init( std::shared_ptr<Muspelheim::Renderer> renderer );

    bool Load( std::wstring const & filename );

	bool Update();

	void SetPosition( glm::vec3 const & position );

private:
	std::weak_ptr<Muspelheim::RenderObject> m_RenderObject;
	Muspelheim::SurfaceID m_SurfaceID;
};
