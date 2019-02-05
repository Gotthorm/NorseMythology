// CONSOLEPARSER.H

#ifndef _CONSOLEPARSER_H_
#define _CONSOLEPARSER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Niflheim { class MessageManager; }

class ConsoleParser
{
public:
	// Use the default implementation
	ConsoleParser( std::weak_ptr<Niflheim::MessageManager> const & messageManager );

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
};

#endif // _CONSOLEPARSER_H_