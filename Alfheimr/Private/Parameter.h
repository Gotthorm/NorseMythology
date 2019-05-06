// PARAMETER.H

#pragma once

#include "Alfheimr.h"

namespace Alfheimr
{
	class Parameter
	{
	public:
		Parameter( ParameterList::ParameterType type ) : m_Type( type ) {}

		virtual ~Parameter() = default;

		ParameterList::ParameterType GetType() { return m_Type; }

	private:
		ParameterList::ParameterType m_Type;
	};

	template< typename T >
	class TypedParameter : public Parameter
	{
	public:
		TypedParameter( ParameterList::ParameterType type, const T& data ) : Parameter( type ), m_Data( data ) {}

		T GetData() { return m_Data; }

		void SetData( T const data ) { m_Data = data; }

	private:
		T m_Data;
	};
}