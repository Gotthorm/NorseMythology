// VOLSTAGG.H

#ifndef _VOLSTAGG_H_
#define _VOLSTAGG_H_

#include <string>
#include <memory>
#include "Muspelheim.h"

class Volstagg
{
public:
	Volstagg( Muspelheim::SurfaceID id );
    virtual ~Volstagg();

	bool Init( Muspelheim::Renderer& renderer );

    bool Load( const std::string& fileName );

	bool Update();

private:
	std::weak_ptr<Muspelheim::RenderObject> m_RenderObject;
	Muspelheim::SurfaceID m_SurfaceID;
	//unsigned int m_DisplacementTexture = 0;
};

#endif // _VOLSTAGG_H_

