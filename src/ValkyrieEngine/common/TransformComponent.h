#pragma once

#include "ValkyrieEngine/core/GameComponent.hpp"
#include "ValkyrieEngine/common/Vector.h"

namespace vlk
{
	struct TransformComponent2D : public Component<TransformComponent2D>
	{
		TransformComponent2D(IEntity* e);

		void Translate(Vector2 translation);
		void Scale(Vector2 scale);
		void Scale(Float scale);
		void Rotate(Float rotation);

		//Order of transformation:
		//Translate -> Scale -> Rotate

		Vector2 location;
		Vector2 scale;
		Float rotation;
	};
	/*
	class TransformComponent3D
	{
		public:
		TransformComponent3D();
	};
	*/
}