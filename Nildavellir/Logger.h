#pragma once

#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include "MessageClient.h"

// The logger system is a singleton with the main purpose of logging information to a file.
// It also retains a cache of recent log data that can be extracted by other systems, like the console.

class Logger : public MessageClient
{
public:
	Logger();

	virtual ~Logger();

	bool Initialize( const std::wstring& fileName );

	void Shutdown();

	// Override MessageClient::ReceiveMessage
	virtual void ReceiveMessage( const Message& message );

private:
	std::wofstream m_FileOutput;
};