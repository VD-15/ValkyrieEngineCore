#include "ByteBuffer.hpp"

using namespace vlk;

ByteBuffer::ByteBuffer(Size size) :
	data(new Byte[size]),
	size(size),
	position(data)
{

}

ByteBuffer::~ByteBuffer()
{
	delete[] data;
}

void ByteBuffer::SetPos(Size offset)
{
	if (offset >= size)
	{
		throw std::exception("Buffer offset out of range.");
	}
	else
	{
		position = data + offset;
	}
}