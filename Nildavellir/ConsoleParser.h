// CONSOLEPARSER.H

#ifndef _CONSOLEPARSER_H_
#define _CONSOLEPARSER_H_

#include <string>
#include <vector>
#include <unordered_map>

class ConsoleParser
{
public:
	// Use the default implementation
	ConsoleParser() = default;

	// Use the default implementation
	virtual ~ConsoleParser() = default;

	//
	void Execute( const std::wstring& commandLine );

private:
	// Remove the default assignment operator
	ConsoleParser & operator =( const ConsoleParser & ) = delete;

	// Remove the copy constructor
	ConsoleParser( const ConsoleParser & ) = delete;

	// 
	int Tokenize( const std::wstring& inputString, std::vector<std::wstring>& tokens );

	//
	bool GetCommandToken( std::wstring& commandBuffer, std::wstring& token );
};

#endif // _CONSOLEPARSER_H_