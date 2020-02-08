#include "GLBuffer.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

using namespace vlk::GL;

GLBuffer::GLBuffer(UInt size, UInt usage) :
	handle(GenBuffer()),
	size(size)
{
	UInt currentBinding = GetBoundBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, this->handle);
	glBufferData(GL_ARRAY_BUFFER, this->size, nullptr, usage);
	glBindBuffer(GL_ARRAY_BUFFER, currentBinding);

	this->target = 0;
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLBuffer::Bind(UInt _target)
{
	if (this->target != 0)
	{
		throw std::exception("This buffer is already bound to a different target.");
	}

	//Insert or access pointer
	UInt boundBuffer = GetBoundBuffer(_target);

	if (boundBuffer != 0)
	{
		throw std::exception("A different buffer is already bound to the target.");
	}

	glBindBuffer(_target, this->handle);
	this->target = _target;
}

void GLBuffer::Unbind()
{
	if (this->target == 0)
	{
		throw std::exception("This buffer is not bound to a target.");
	}

	UInt currentBinding = GetBoundBuffer(this->target);

	if (currentBinding != 0)
	{
		throw std::exception("This buffer is not bound to its target.");
	}

	glBindBuffer(this->target, 0);
	this->target = 0;
}

void GLBuffer::Fill(const ByteBuffer& data)
{
	if (this->target == 0)
	{
		throw std::exception("This buffer is not bound to a target.");
	}

	if (data.size > this->size)
	{
		throw std::exception("Buffer data is too big.");
	}

	if (GetBoundBuffer(this->target) != this->handle)
	{
		throw std::exception("A different buffer is bound to the target.");
	}

	glBufferSubData(this->target, 0, data.size, data.data);
}

UInt GLBuffer::GenBuffer()
{
	UInt handle = 0;
	glGenBuffers(1, &handle);
	return handle;
}