#pragma once

#include "ValkyrieEngine/core/ValkyrieEngine.h"

namespace vlk
{
	class ByteBuffer final
	{
		public:
		ByteBuffer();
		~ByteBuffer();

		ByteBuffer(const ByteBuffer& other) = delete;
		ByteBuffer(ByteBuffer&& other) = delete;
		ByteBuffer& operator=(const ByteBuffer& other) = delete;

		inline const ULong Size() const { return size; }
		inline const void* Data() const { return data; }
		inline void* Data() { return data; }

		void Allocate(ULong size);
		void SetPos(ULong offset);
		void Reset();

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
		T& GetAt(ULong offset)
		{
			Byte* bp = data + offset;

			#ifdef _DEBUG
			if (bp > data + size)
			{
				throw std::exception("Buffer offset out of range.");
			}
			#endif

			return *reinterpret_cast<T*>(data + offset);
		}

		template<class T>
		T& Get(ULong index)
		{
			T* tp = reinterpret_cast<T*>(data);

			#ifdef _DEBUG
			if (tp > data + size)
			{
				throw std::exception("Buffer index out of range.");
			}
			#endif

			return *(tp + index);
		}

		template<class T>
		T& Get()
		{
			return reinterpret_cast<T*>(position);
		}

		template<class T>
		T* AdvanceBy()
		{
			#ifdef _DEBUG
			if (position + sizeof(T) > data + size)
			{
				throw std::exception("Buffer position going out of range.");
			}
			#endif

			position += sizeof(T);
			return reinterpret_cast<T*>(position);
		}

		private:
		Byte* data;
		ULong size;
		Byte* position;
	};
}