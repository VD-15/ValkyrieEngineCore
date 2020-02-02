#include "StringCollection.h"

using namespace vlk;

StringCollection::StringCollection()
{
	data = nullptr;

	lengths = nullptr;

	count = 0;
}

StringCollection::StringCollection(std::vector<std::string> strings)
{
	count = static_cast<UInt>(strings.size());
	data = new const char* [count];
	lengths = new Int[count];

	for (UInt i = 0; i < count; i++)
	{
		data[i] = DuplicateString(strings[i].c_str());
		lengths[i] = static_cast<Int>(strings[i].size());
	}
}

StringCollection::StringCollection(const char** strings, UInt stringCount)
{
	data = new const char* [stringCount];
	lengths = new Int[stringCount];
	count = stringCount;

	for (UInt i = 0; i < stringCount; i++)
	{
		data[i] = DuplicateString(strings[i]);
		lengths[i] = static_cast<UInt>(strlen(strings[i]));
	}
}

StringCollection::StringCollection(const StringCollection& old)
{
	data = new const char* [old.count];
	lengths = new Int[old.count];
	count = old.count;

	for (UInt i = 0; i < count; i++)
	{
		data[i] = DuplicateString(old.data[i]);
		lengths[i] = old.lengths[i];
	}
}

StringCollection::StringCollection(StringCollection&& other) noexcept
{
	data = other.data;
	lengths = other.lengths;
	count = other.count;

	other.data = nullptr;
	other.lengths = nullptr;
	other.count = 0;
}

StringCollection::~StringCollection()
{
	for (UInt i = 0; i < count; i++)
	{
		delete[] data[i];
	}

	delete[] data;
	delete[] lengths;
}
