// PARAMETERLISTIMPLEMENTATION.H

#pragma once

#include <string>
#include <vector>
#include "Alfheimr.h"

namespace Alfheimr
{
	// Prototypes
	class Parameter;

	class ParameterListImplementation : public ParameterList
	{
	public:
		ParameterListImplementation() = default;
		virtual ~ParameterListImplementation() = default;

		virtual ParameterType GetType( unsigned int index ) const override;

		virtual unsigned int GetCount() const override { return static_cast< unsigned int >( m_List.size() ); }

		virtual bool GetValue( unsigned int index, int & param ) const override;
		virtual bool GetValue( unsigned int index, bool & param ) const override;
		virtual bool GetValue( unsigned int index, float & param ) const override;
		virtual bool GetValue( unsigned int index, unsigned int & param ) const override;
		virtual bool GetValue( unsigned int index, std::wstring & param ) const override;

		virtual bool SetValue( unsigned int index, int param ) override;
		virtual bool SetValue( unsigned int index, bool param ) override;
		virtual bool SetValue( unsigned int index, float param ) override;
		virtual bool SetValue( unsigned int index, unsigned int param ) override;
		virtual bool SetValue( unsigned int index, std::wstring const & param ) override;

		void Clear();

	protected:
		friend ParameterList;
		std::vector<Parameter*> m_List;
	};
}
