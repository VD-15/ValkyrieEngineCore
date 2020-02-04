#pragma once

#include "ValkyrieEngine/core/Event.hpp"
#include "ValkyrieEngine/core/Content.hpp"
#include "ValkyrieEngine/window/Window.h"
#include "ValkyrieEngine/graphics/Texture.h"
#include "ValkyrieEngine/common/ByteBuffer.hpp"

#include <map>

namespace vlk
{
	//Helper class for OpenGL buffers
	class GLBuffer
	{
		public:
		GLBuffer(UInt size, UInt usage);
		~GLBuffer();

		GLBuffer(const GLBuffer&) = delete;
		GLBuffer(GLBuffer&&) = delete;
		GLBuffer& operator=(const GLBuffer&) = delete;
		GLBuffer& operator=(GLBuffer&&) = delete;

		void Bind(UInt target);
		void Unbind();

		void Fill(const ByteBuffer& data);

		const UInt size;
		const UInt handle;
		inline UInt GetTarget() const { return this->target; }

		private:
		static UInt GenBuffer();
		static UInt GetBoundBuffer(UInt target);
		//TODO: bindings again

		UInt target;
	};

	struct GLRendererArgs
	{

	};

	class GLRenderer :
		public EventListener<PostUpdateEvent>,
		public EventListener<WindowFramebufferEvent>,
		public EventListener<ContentLoadedEvent<Texture2D>>,
		public EventListener<ContentUnloadedEvent<Texture2D>>
	{
		public:
		GLRenderer(const GLRendererArgs& args);
		~GLRenderer();

		private:
		void OnEvent(const PostUpdateEvent& ev);					//Rendering
		void OnEvent(const WindowFramebufferEvent& ev);				//Resize viewport
		void OnEvent(const ContentLoadedEvent<Texture2D>& ev);		//Load texture
		void OnEvent(const ContentUnloadedEvent<Texture2D>& ev);	//Unload texture
	};
}
