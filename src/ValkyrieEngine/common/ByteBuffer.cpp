#include "ByteBuffer.hpp"

using namespace vlk;

ByteBuffer::ByteBuffer()
{
	data = nullptr;
	size = 0;
	position = nullptr;
}

ByteBuffer::~ByteBuffer()
{
	delete[] data;
}

void ByteBuffer::Allocate(ULong size)
{
	data = new Char[size];
	this->size = size;
	position = data;
}

void ByteBuffer::SetPos(ULong offset)
{
	if (data + offset >= data + size)
	{
		throw std::exception("Buffer offset out of range.");
	}
	else
	{
		position = data + offset;
	}
}

void ByteBuffer::Reset()
{
	position = data;
}