// LOKI.H

#ifndef _LOKI_H_
#define _LOKI_H_

//#include "VectorMath.h"
#include <string>
#include <memory>
#include "Muspelheim.h"

class Loki
{
public:
	Loki( Muspelheim::SurfaceID id );
    virtual ~Loki();

	bool Init( Muspelheim::Renderer& renderer );

    bool Load( const std::string& filename );

	bool Update();

	void SetPosition( const glm::vec4& position );

private:
	std::weak_ptr<Muspelheim::RenderObject> m_RenderObject;
	Muspelheim::SurfaceID m_SurfaceID;
};

#endif // _LOKI_H_

