// PARAMETER.H

#pragma once

#include "Alfheimr.h"

class Parameter
{
public:
	Parameter( Alfheimr::ParameterList::ParameterType type  ) : m_Type( type ) {}

	virtual ~Parameter() = default;

	Alfheimr::ParameterList::ParameterType GetType() { return m_Type; }

private:
	Alfheimr::ParameterList::ParameterType m_Type;
};

template< typename T >
class TypedParameter : public Parameter
{
public:
	TypedParameter( Alfheimr::ParameterList::ParameterType type, const T& data ) : Parameter( type ), m_Data( data ) {}

	T GetData() { return m_Data; }

	void SetData( T const data ) { m_Data = data; }

private:
	T m_Data;
};