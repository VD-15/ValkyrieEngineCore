#pragma once

#include "ValkyrieEngine.h"
#include "SingletonAllocator.hpp"

//TODO: Do we need any of this?
//'Modules' (or at least their logic) can and should just be treated as systems

namespace vlk
{
	class IModuleQueue
	{
		public:
		IModuleQueue() = default;
		virtual ~IModuleQueue() = default;

		virtual void Init() = 0;
		virtual void Destroy() = 0;
	};

	template <typename T, typename U>
	class ModuleQueue : public IModuleQueue
	{
		private:
		U args;
		T* instance;

		public:
		ModuleQueue<T, U>() = delete;
		~ModuleQueue<T, U>() = default;
		ModuleQueue<T, U>(const ModuleQueue<T, U>&) = delete;
		ModuleQueue<T, U>(ModuleQueue<T, U>&&) = delete;
		ModuleQueue<T, U>& operator=(const ModuleQueue<T, U>&) = delete;
		ModuleQueue<T, U>& operator=(ModuleQueue<T, U>&&) = delete;

		ModuleQueue<T, U>(const U& _args) :
			args(_args),
			instance(nullptr)
		{

		}

		private:
		void Init() final override
		{
			this->instance = SingletonAllocator<T>::AllocateNew(this->args);
		}

		void Destroy() final override
		{
			SingletonAllocator<T>::Delete(this->instance);
		}
	};

	template <typename T, typename U>
	void RegisterModule(const U& args)
	{
		static ModuleQueue<T, U>* queue = nullptr;

		if (queue)
		{
			LogWarning("Module " + TypeToString<T>(), "Module already registered.");
		}

		queue = SingletonAllocator<ModuleQueue<T, U>>::AllocateNew(args);
		RegisterModuleQueue(static_cast<IModuleQueue*>(queue));
	}

	void RegisterModuleQueue(IModuleQueue* m);

	//Initializes the following modules with their default args:
	//VLKRandom
	//Window
	//Mouse
	//Keyboard
	//GLRenderer
	void RegisterDefaultModules();
}