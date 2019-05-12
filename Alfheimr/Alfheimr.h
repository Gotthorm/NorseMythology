
#pragma once  

#include <memory>
#include <string>
#include <functional>

// Prototypes
namespace Niflheim		{ class MessageManager; }
namespace Muspelheim	{ class Renderer; }
namespace Helheimr		{ class Input; }

namespace Alfheimr
{
	class ParameterList
	{
	public:
		enum class ParameterType
		{
			INVALID,
			STRING,
			INT,
			FLOAT,
			UINT,
			BOOL,
		};

		virtual ParameterType GetType( unsigned int index ) const = 0;

		virtual unsigned int GetCount() const = 0;

		virtual bool GetValue( unsigned int index, int & param ) const = 0;
		virtual bool GetValue( unsigned int index, bool & param ) const = 0;
		virtual bool GetValue( unsigned int index, float & param ) const = 0;
		virtual bool GetValue( unsigned int index, std::wstring & param ) const = 0;
		virtual bool GetValue( unsigned int index, unsigned int & param ) const = 0;

		virtual bool SetValue( unsigned int index, int param ) = 0;
		virtual bool SetValue( unsigned int index, bool param ) = 0;
		virtual bool SetValue( unsigned int index, float param ) = 0;
		virtual bool SetValue( unsigned int index, std::wstring const & param ) = 0;
		virtual bool SetValue( unsigned int index, unsigned int param ) = 0;
	};

	/// \brief A window for logging and run time variable tweaking
	///
	/// The Console class controls a special window whose visibility is toggled with the '`' or '~' keys.
	/// It is used for monitoring logged messages in real time and to also allow for querying and modifying run time variables.
	class Console
	{
	public:
		static std::shared_ptr<Console> Create( std::weak_ptr<Niflheim::MessageManager> const & messageManager, std::weak_ptr<Muspelheim::Renderer> const & renderer );

		/// \brief Create the console window.
		///
		/// Create and activate the console window.  
		/// The console will immediately begin receiving messages after this call. 
		/// \param width The width of the parent window.
		/// \param height The height of the parent window.
		/// \param verticalClipSize The vertical percentage of the parent's height that will be used by this console window. [0.0 > verticalClipSize <= 1.0]%
		/// \return Returns a boolean indicating whether the console has been successfully initialized.
		virtual bool Initialize( int width, unsigned int height, float verticalClipSize ) = 0;

		/// \brief Get the console cache size
		///
		/// This gets the number of lines that are cached in the console window.
		/// The value set here will be the number of full lines without taking into account any wrapping.
		/// \param lineCount The number of text lines that are being be stored in the cache.
		/// \return Returns a boolean indicating whether the call was successful.
		virtual bool GetMaximumLineCount( unsigned int & lineCount ) = 0;

		/// \brief Set the console cache size
		///
		/// This sets the number of lines that are cached in the console window.
		/// The value set here will be the number of full lines without taking into account any wrapping.
		/// Setting the size smaller than the height of the console will cause the given size to be clamped to the console line height.
		/// \param cacheSize The number of text lines that will be stored in the cache.
		/// \return Returns a boolean indicating whether the call was successful.
		virtual bool SetMaximumLineCount( unsigned int lineCount ) = 0;

		/// \brief Notify the console of the current parent window size.
		///
		/// Call this whenever the parent window changes dimensions.
		/// The values used are typically the dimension of the parent windows client area dimensions.
		/// \param width The width of the parent window.
		/// \param height The height of the parent window.
		virtual void UpdateWindowSize( unsigned int width, unsigned int height ) = 0;

		/// \brief Get the text scale
		///
		/// Get the current horizontal and vertical scale of the text.
		/// \param widthScale
		/// \param heightScale
		/// \return Returns a boolean indicating whether the call was successful.
		virtual bool GetTextScale( float & widthScale, float & heightScale ) = 0;

		/// \brief Set the text scale
		///
		/// Set the horizontal and vertical scale of the text.
		/// The default values are (1.0, 1.0)
		/// When you change these values the console will adjust the number of characters displayed both vertically and horizontally to match the new dimensions.
		/// \param widthScale
		/// \param heightScale
		/// \return Returns a boolean indicating whether the call was successful.
		virtual bool SetTextScale( float widthScale, float heightScale ) = 0;

		/// \brief Render the console window
		///
		/// This is called by the graphics system every render frame.
		virtual void Render() = 0;

		/// \brief Set the console visible or non visible.
		///
		/// Set the console visible or non visible. 
		/// \param visible
		virtual void SetVisible( bool visible ) = 0;

		/// \brief Get the visibility status of the console window.
		///
		/// Get the visibility status of the console window.
		/// \return Returns a boolean indicating whether the console window is visible or not.
		virtual bool IsVisible() = 0;

		/// \brief Register a console command
		///
		/// Register a call back method that will be called by the defined command string.
		/// \param functionName
		/// \param callback
		/// \param params
		///
		/// An example using the call back method that expects a single boolean as its parameter
		/// \code{.cpp}
		/// RegisterCommand( L"vsync", std::bind( &Framework::VSync_Callback, this, std::placeholders::_1 ), Alfheimr::ParameterList::Create( 1, Alfheimr::ParameterList::ParameterType::BOOL ) );
		/// \endcode
		virtual bool RegisterCommand( std::wstring const & functionName, std::function<void( ParameterList const & )> callback, std::shared_ptr<const ParameterList> params ) = 0;
	};
}


