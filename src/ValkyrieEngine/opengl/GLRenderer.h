#pragma once

#include "ValkyrieEngine/core/Event.hpp"
#include "ValkyrieEngine/core/Content.hpp"
#include "ValkyrieEngine/window/Window.h"
#include "ValkyrieEngine/graphics/Texture.h"

namespace vlk
{
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
