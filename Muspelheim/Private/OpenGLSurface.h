#pragma once  

#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "Muspelheim.h"
#include "VectorMath.h"
#include "OpenGLText.h"

namespace Muspelheim
{
	class OpenGLRenderObject;
	class OpenGLShader;
	class OpenGLLightObject;

	class OpenGLSurface
	{
	public:
		//
		OpenGLSurface() = default;

		//
		~OpenGLSurface() = default;

		//
		bool Init( std::weak_ptr<OpenGLSurface> weakSelf, unsigned short width, unsigned short height, std::weak_ptr<Niflheim::MessageManager> const & messageManager );

		//
		bool SetSize( unsigned short width, unsigned short height );

		//
		void SetShader( OpenGLShader* shader );

		//
		void SetTextShader( OpenGLShader* shader );

		//
		void SetColor( const glm::vec4& color );

		//
		const glm::vec4& GetColor() { return m_Color; }

		//
		void SetDepth( unsigned char depth );

		// 
		unsigned char GetDepth() { return m_Depth; }

		//
		bool QueueRender( unsigned short renderObjectID );

		//
		void RenderAll( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );

		//
		bool DrawString( const std::wstring& textString, unsigned int color, unsigned short posX, unsigned short posY, Renderer::TextAlignment alignment );

		//
		bool DrawStringBuffer( TextBuffer const & textBuffer );

		//
		std::shared_ptr<RenderObject> CreateRenderObject();

		//
		void AddLightObject( std::weak_ptr<OpenGLLightObject> lightObject );
		std::shared_ptr<LightObject> CreateLightObject();

		//
		//bool GetRenderBackgroundEnabled() { return m_RenderBackground; }
		//void SetRenderBackgroundEnabled( bool render ) { m_RenderBackground = render; }

		//
		bool GetIsVisible() { return m_Visible; }
		void SetIsVisible( bool visible ) { m_Visible = visible; }

		unsigned int GetFontWidth() { return m_Text.GetFontWidth(); }
		unsigned int GetFontHeight() { return m_Text.GetFontHeight(); }

		// Parameters are percentage values [0 <=> 1]
		// Full screen (default) would be (0, 0, 1, 1)
		void SetClipping( float leftX, float topY, float rightX, float bottomY );

		void SetTextScale( float widthScale, float heightScale );

	private:
		unsigned char m_Depth = 0;
		unsigned short m_Width = 0;
		unsigned short m_Height = 0;

		// Flag to control whether actual surface is to be rendered or not
		// When rendered, it will use m_Color
		//bool m_RenderBackground = false;

		// Flag for disabling the rendering of this surface
		bool m_Visible = true;

		// The color of the surface background
		// When the alpha value is zero the background will not be rendered
		glm::vec4 m_Color = { 0.0f, 0.0f, 0.0f, 0.0f };

		// The vertical clip value used when rendering the background [ 0.0 <=> -1.0 ]
		// The value is the percentage from the vertical center of the screen to the bottom
		// 1.0, -1.0 is full screen
		// 1.0, -0.5 is 75% vertical beginning at the top of the screen
		glm::vec4 m_ClipSize = { -1.0f, -1.0f, 1.0f, 1.0f };

		// The text object for rendering text onto the surface
		OpenGLText m_Text;

		// The optional shader for rendering the actual surface
		// It is valid for this to be empty
		OpenGLShader* m_Shader = nullptr;

		// Used for populating child render objects
		std::weak_ptr<OpenGLSurface> m_WeakSelf;

		// The list of render objects that will be rendered this frame
		std::vector<std::shared_ptr<OpenGLRenderObject>> m_RenderQueue;

		// The list of all render objects owned by this surface
		std::vector<std::shared_ptr<OpenGLRenderObject>> m_RenderObjects;
	};
}



