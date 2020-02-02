#pragma once

#include <vector>
#include <string>

#include "../core/ValkyrieEngine.h"

namespace vlk
{
	class StringCollection
	{
		public:
		StringCollection();
		StringCollection(std::vector<std::string> strings);
		StringCollection(const char** strings, UInt stringCount);
		StringCollection(const StringCollection& old);
		StringCollection(StringCollection&& other) noexcept;
		virtual ~StringCollection();

		inline const char* operator[] (UInt i) const { return data[i]; }

		inline const char* const* Data() const { return data; }
		inline const Int* Lengths() const { return lengths; }
		inline UInt Count() const { return count; }

		private:
		const char** data;
		Int* lengths;
		UInt count;
	};
}