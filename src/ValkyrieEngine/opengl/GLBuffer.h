#pragma once

#include "ValkyrieEngine/common/ByteBuffer.hpp"

namespace vlk
{
	namespace GL
	{
		//Helper class for OpenGL buffers
		class GLBuffer
		{
			public:
			GLBuffer() = delete;
			GLBuffer(UInt size, UInt usage);
			~GLBuffer();

			GLBuffer(const GLBuffer&) = delete;
			GLBuffer(GLBuffer&&) = delete;
			GLBuffer& operator=(const GLBuffer&) = delete;
			GLBuffer& operator=(GLBuffer&&) = delete;

			inline operator UInt() { return id; }

			void Bind(UInt target);
			void Unbind();

			void Fill(const ByteBuffer& data);

			inline UInt GetSize() const { return this->size; }
			inline UInt GetTarget() const { return this->target; }

			private:
			static UInt GenBuffer();
			static UInt GetBoundBuffer(UInt target);
			//TODO: bindings again

			UInt target;
			const UInt id;
			const UInt size;
		};
	}
}