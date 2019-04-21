#pragma once

#include <vector>

namespace Jotunheim
{
	template <class T>
	class RingBuffer
	{
	public:
		explicit RingBuffer( size_t size ) : m_MaxSize( size ), m_Buffer( size )
		{
		}

		void Push( T item )
		{
			if ( m_Buffer.size() <= m_HeadIndex )
			{
				m_Buffer.push_back( item );
			}
			else
			{
				m_Buffer[ m_HeadIndex ] = item;
			}

			if ( m_Full ) m_TailIndex = ( m_TailIndex + 1 ) % m_MaxSize;
			m_HeadIndex = ( m_HeadIndex + 1 ) % m_MaxSize;
			m_Full = m_HeadIndex == m_TailIndex;
		}

		T Pop()
		{
			if ( Empty() ) return T();
			T item = m_Buffer[ m_TailIndex ];
			m_Full = false;
			m_TailIndex = ( m_TailIndex + 1 ) % m_MaxSize;
			return item;
		}

		void Reset() { m_HeadIndex = m_TailIndex; m_Full = false; }

		bool Empty() const { return ( !m_Full && ( m_HeadIndex == m_TailIndex ) ); }

		bool Full() const { return m_Full; }

		size_t Capacity() const { return m_MaxSize; }

		size_t Size() const
		{
			size_t size = m_MaxSize;
			if ( !m_Full )
			{
				if ( m_HeadIndex >= m_TailIndex )
				{
					size = m_HeadIndex - m_TailIndex;
				}
				else
				{
					size = m_MaxSize + m_HeadIndex - m_TailIndex;
				}
			}
			return size;
		}

		void Resize( size_t size )
		{
			while ( Size() > size ) Pop();
			std::vector<T> tempBuffer( Size() );
			while ( Size() > 0 ) tempBuffer.push_back( Pop() );
			m_TailIndex = 0;
			m_HeadIndex = tempBuffer.size();
			m_Buffer.swap( tempBuffer );
			m_MaxSize = size;
		}

		T & operator[] ( int index )
		{
			if ( index >= Size() ) throw;
			return m_Buffer[ ( m_TailIndex + index ) % m_MaxSize ];
		}

	private:
		std::vector<T> m_Buffer;
		size_t m_HeadIndex = 0;
		size_t m_TailIndex = 0;
		size_t m_MaxSize;
		bool m_Full = false;
	};
}
