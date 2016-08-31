// CONSOLEPARAMETERLIST.H

#ifndef _CONSOLEPARAMETERLIST_H_
#define _CONSOLEPARAMETERLIST_H_

#include <string>
#include <vector>

//#include <tuple>

//#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
//#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))

//#define NUMARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
//#define REGISTER(...)  (ConsoleCommands::GetInstance()->RegisterCommand(NUMARGS(__VA_ARGS__), __VA_ARGS__))

class ConsoleParameter
{
public:
	enum class ParameterType
	{
		INVALID,
		STRING,
		INT,
		FLOAT,
		UINT,
	};

	ConsoleParameter( ParameterType type ) : m_Type( type ) {}

	virtual ~ConsoleParameter() = default;

	ParameterType GetType() { return m_Type; }

private:
	ParameterType m_Type;
};

template< typename T >
class TypedConsoleParameter : public ConsoleParameter
{
public:
	TypedConsoleParameter( ParameterType type, const T& data ) : ConsoleParameter( type ), m_Data( data ) {}

	T GetData() { return m_Data; }

	void SetData( T data ) { m_Data = data; }

private:
	T m_Data;
};

class ConsoleParameterList
{
public:
	ConsoleParameterList() = default;
	ConsoleParameterList( int paramCount, ConsoleParameter::ParameterType... );

	ConsoleParameter::ParameterType GetParameterType( unsigned int index ) const;

	ConsoleParameter* GetParameterValue( unsigned int index ) const;

	unsigned int GetCount() const { return unsigned int(m_List.size()); }

	bool SetParameterValue( unsigned int index, int param );
	bool SetParameterValue( unsigned int index, float param );
	//bool SetParameterValue( unsigned int index, const std::string* param );
	bool SetParameterValue( unsigned int index, unsigned int param );

private:
	std::vector<ConsoleParameter*> m_List;
};

#endif // _CONSOLEPARAMETERLIST_H_