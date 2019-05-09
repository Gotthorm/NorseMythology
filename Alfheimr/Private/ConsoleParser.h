// CONSOLEPARSER.H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Niflheim { class MessageManager; }

namespace Alfheimr
{
	class ConsoleCommandManager;

	class ConsoleParser
	{
	public:
		// Use the default implementation
		ConsoleParser( std::weak_ptr<Niflheim::MessageManager> const & messageManager, std::weak_ptr<ConsoleCommandManager> const & commandManager );

		// Use the default implementation
		virtual ~ConsoleParser() = default;

		//
		void Execute( const std::wstring& commandLine );

	private:
		// Remove the default assignment operator
		//ConsoleParser & operator =( const ConsoleParser & ) = delete;

		// Remove the copy constructor
		//ConsoleParser( const ConsoleParser & ) = delete;

		// 
		int Tokenize( const std::wstring& inputString, std::vector<std::wstring>& tokens );

		//
		bool GetCommandToken( std::wstring& commandBuffer, std::wstring& token );

		std::weak_ptr<Niflheim::MessageManager> m_MessageManager;
		std::weak_ptr<ConsoleCommandManager> m_CommandManager;
	};
}
