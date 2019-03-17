#pragma once  

#ifdef MAKE_DLL  
#define EXPORT __declspec(dllexport)   
#else  
#define EXPORT __declspec(dllimport)   
#endif 

#include <string>
#include <memory>
#include "Platform.h"
#include "Vanaheimr.h"

namespace Niflheim { class MessageManager; }

namespace Muspelheim
{
	class Renderer;

	typedef unsigned char SurfaceID;
	const SurfaceID InvalidSurface = 0;

	class EXPORT RenderObject : public Vanaheimr::Object3D
	{
	public:
		enum PolyMode { TRIANGLES, PATCH };

		//
		virtual bool SetShader( unsigned int shaderID ) = 0;

		//
		virtual bool LoadData( unsigned int size, void* data, unsigned int objectCount ) = 0;

		//
		virtual bool LoadTexture( const unsigned char* imageData, unsigned int height, unsigned int width, bool hasAlpha ) = 0;

		//
		virtual bool Render() = 0;

		//
		virtual void SetWindingOrderClockwise( bool clockwise ) = 0;
		virtual void SetPolyLineMode( bool lineMode ) = 0;
		virtual void SetPolyBackFace( bool backFace ) = 0;
		virtual void SetPolyMode( PolyMode polyMode ) = 0;
	};

	class EXPORT LightObject : public Vanaheimr::Object3D
	{
	public:
		//
		virtual bool SetAmbient( float r, float g, float b, float a ) = 0;

		//
		virtual bool SetDiffuse( float r, float g, float b, float a ) = 0;

		//
		virtual bool SetSpecular( float r, float g, float b, float a ) = 0;

		//
		virtual bool SetEmission( float r, float g, float b, float a ) = 0;

		//
		virtual bool GetEnabled() = 0;

		//
		virtual void SetEnabled( bool enabled ) = 0;
	};

	class EXPORT Renderer
	{
	public:
		enum TextAlignment { TEXT_LEFT, TEXT_RIGHT, TEXT_MIDDLE };

		//
		virtual bool Initialize( const Platform::WindowHandle& hWindow ) = 0;

		//
		virtual void BeginRender( const glm::mat4& viewMatrix ) = 0;

		//
		virtual void EndRender() = 0;

		//
		virtual void SetWindowSize( unsigned short width, unsigned short height ) = 0;

		//
		virtual std::wstring GetVersionInformation() = 0;

		//
		virtual void AddMessaging( const std::weak_ptr<Niflheim::MessageManager>& messageManager ) = 0;

		//
		virtual unsigned int LoadShader( const std::wstring& shaderName ) = 0;

		//
		virtual unsigned int LoadTexture( const unsigned char* imageData, unsigned int height, unsigned int width, bool hasAlpha ) = 0;

		//
		virtual bool GetVSyncEnabled() = 0;

		//
		virtual void SetVSyncEnabled( bool enable ) = 0;

		//
		static std::shared_ptr<Renderer> Create();

		//
		// Surface Methods
		//

		//
		virtual bool CreateSurface( SurfaceID& surfaceID ) = 0;

		//
		virtual bool DestroySurface( SurfaceID surfaceID ) = 0;

		// 
		virtual bool GetSurfaceDepth( SurfaceID surfaceID, unsigned char& depth ) = 0;

		//
		virtual bool SetSurfaceDepth( SurfaceID surfaceID, unsigned char depth ) = 0;

		//
		virtual bool GetSurfaceColor( SurfaceID surfaceID, glm::vec4& color ) = 0;

		//
		virtual bool SetSurfaceColor( SurfaceID surfaceID, const glm::vec4& color ) = 0;

		//
		virtual bool GetSurfaceVisible( SurfaceID surfaceID, bool& visible ) = 0;

		//
		virtual bool SetSurfaceVisible( SurfaceID surfaceID, bool visible ) = 0;

		//
		virtual bool SetSurfaceShader( SurfaceID surfaceID, unsigned int shaderID ) = 0;

		//
		virtual bool SetSurfaceTextShader( SurfaceID surfaceID, unsigned int shaderID ) = 0;

		//
		virtual bool GetSurfaceFontSize( SurfaceID surfaceID, unsigned int & width, unsigned int & height ) = 0;

		//
		virtual bool DrawSurfaceString( SurfaceID surfaceID, const std::wstring& textString, unsigned short posX, unsigned short posY, TextAlignment alignment ) = 0;

		//
		virtual bool DrawSurfaceStringBuffer( SurfaceID surfaceID, wchar_t const * pTextBuffer, unsigned int size ) = 0;

		//
		virtual std::shared_ptr<RenderObject> CreateSurfaceRenderObject( SurfaceID surfaceID ) = 0;

		//
		virtual std::shared_ptr<LightObject> CreateSurfaceLightObject( SurfaceID surfaceID ) = 0;

		//
		virtual bool SetSurfaceClipping( SurfaceID surfaceID, float x, float y, float width, float height ) = 0;
	
		//
		virtual bool SetSurfaceTextScale( SurfaceID surfaceID, float widthScale, float heightScale ) = 0;
	};
}



