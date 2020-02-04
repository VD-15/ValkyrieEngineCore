#pragma once

#include "ValkyrieEngine/core/ValkyrieEngine.h"

namespace vlk
{
	class ByteBuffer final
	{
		public:
		ByteBuffer(Size size);
		~ByteBuffer();

		ByteBuffer(const ByteBuffer&) = delete;
		ByteBuffer(ByteBuffer&&) = delete;
		ByteBuffer& operator=(const ByteBuffer&) = delete;
		ByteBuffer& operator=(ByteBuffer&&) = delete;

		Byte* const data;
		const Size size;

		inline Byte* GetPos() { return this->position; }
		inline const Byte* GetPos() const { return this->position; }

		void SetPos(Size offset);

		template<class T>
		void Put(const T& t)
		{
			#ifdef _DEBUG
			if (position + sizeof(T) > data + size)
			{
				throw std::exception("Buffer position out of range.");
			}
			#endif

			T* tp = reinterpret_cast<T*>(position);
			*tp = t;
			position += sizeof(T);
		}

		template<class T>
		T* GetAt(Size offset)
		{
			Byte* bp = data + offset;

			if (bp > data + size)
			{
				throw std::exception("Buffer offset out of range.");
			}

			return reinterpret_cast<T*>(data + offset);
		}

		template<class T>
		T* Get(Size index)
		{
			T* tp = reinterpret_cast<T*>(data);

			if (tp > data + size)
			{
				throw std::exception("Buffer index out of range.");
			}

			return (tp + index);
		}

		template<class T>
		T* Get()
		{
			return reinterpret_cast<T*>(position);
		}

		template<class T>
		T* AdvanceBy(Size i = 1)
		{
			Size advancement = sizeof(T) * i;

			if (position + advancement > data + size)
			{
				throw std::exception("Buffer position going out of range.");
			}

			position += advancement;
			return reinterpret_cast<T*>(position);
		}

		private:
		Byte* position;
	};
}