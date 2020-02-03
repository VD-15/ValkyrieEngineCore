#pragma once

#include "ValkyrieEngine/common/TransformComponent.h"
#include "ValkyrieEngine/common/Matrix.h"

namespace vlk
{
	struct CameraComponent2D : public Component<CameraComponent2D>
	{
		//The active camera
		static CameraComponent2D* ACTIVE;

		CameraComponent2D(IEntity* e, TransformComponent2D* transform);

		void Activate();

		Matrix4 GetProjection() const;
		Matrix4 GetView() const;

		Boolean autoResize;
		Vector2 viewport;
		Vector2 zoom;
		const TransformComponent2D* transform;
	};
}