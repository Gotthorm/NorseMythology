#pragma once  

#ifdef MAKE_DLL  
#define EXPORT __declspec(dllexport)   
#else  
#define EXPORT __declspec(dllimport)   
#endif 

#include <string>

namespace Valhalla
{
	class EXPORT PNGObject
	{
	public:
		PNGObject() = default;
		virtual ~PNGObject();

		bool Load( const std::string& fileName );

		unsigned int GetHeight() { return m_Height; }
		unsigned int GetWidth() { return m_Width; }

		bool GetAlpha() { return m_Alpha; }

		unsigned int GetSize() { return m_Size; }
		unsigned char* GetData() { return m_Data; }

	private:
		unsigned int m_Height = 0;
		unsigned int m_Width = 0;
		bool m_Alpha = false;

		unsigned int m_Size = 0;
		unsigned char* m_Data = nullptr;
	};

	class EXPORT SBMObject
	{
	public:
		SBMObject() = default;
		virtual ~SBMObject();

		bool Load( const std::string& fileName );

		unsigned int GetSize() { return m_Size; }
		unsigned int GetVertices() { return m_Vertices; }
		unsigned char* GetData() { return m_RenderData; }

	private:
		unsigned int m_Size = 0;
		unsigned int m_Vertices = 0;
		unsigned char* m_RawData = nullptr;
		unsigned char* m_RenderData = nullptr;
	};

	class EXPORT FBXObject
	{
	public:
		FBXObject() = default;
		virtual ~FBXObject();

		bool Load( const std::string& fileName );

		unsigned int GetSize() { return m_Size; }
		unsigned int GetVertices() { return m_Vertices; }
		unsigned char* GetData() { return m_RenderData; }

	private:
		unsigned int m_Size = 0;
		unsigned int m_Vertices = 0;
		unsigned char* m_RenderData = nullptr;
	};

}



