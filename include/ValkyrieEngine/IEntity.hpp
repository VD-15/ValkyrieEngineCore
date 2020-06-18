#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include "ValkyrieEngine/Signal.hpp"

#include <vector>

namespace vlk
{
	class IEntity
	{
		public:
		IEntity() {}
		IEntity(const IEntity&) = delete;
		IEntity(IEntity&&) = delete;
		IEntity& operator=(const IEntity&) = delete;
		IEntity& operator=(IEntity&&) = delete;

		virtual ~IEntity() {}

		virtual void AddSignalListener(ISignalListener* listener) = 0;
		virtual void RemoveSignalListener(ISignalListener* listener) = 0;
		virtual void SendSignal(const Signal& signal) = 0;
	};
}
