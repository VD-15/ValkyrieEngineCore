#pragma once

#include "Event.hpp"

#include <map>

namespace vlk
{
	template <class T>
	struct ContentLoadedEvent
	{
		const std::string& name;
		const T* content;
	};

	template <class T>
	struct ContentUnloadedEvent
	{
		const std::string& name;
		const T* content;
	};

	template <class T>
	struct Content
	{
		Content<T>()
		{
			static_assert(std::is_constructible<T, const std::string&, const std::string&>::value, "T must have a noexcept constructor that takes only two const std::string&");
			static_assert(std::is_nothrow_constructible<T, const std::string&, const std::string&>::value, "T must have a noexcept constructor that takes two const std::string&");
		}

		void AddMetaTag(const std::string& key, const std::string& value)
		{
			meta[key] = value;
		}

		const std::string& GetMetaTag(const std::string& name) const
		{
			auto it = meta.find(name);

			if (it == meta.end())
			{
				throw std::exception("Meta tag not found");
				return "";
			}

			return it->second;
		}

		static const T* Load(const std::string& name, const std::string& path)
		{
			if (loadedContent.find(name) != loadedContent.end())
			{
				throw std::exception("Content already loaded.");
				return nullptr;
			}

			T* t = new T(name, path);

			EventBus<ContentLoadedEvent<T>>::Send({ name, t });

			loadedContent.emplace(name, t);

			LogInfo("Content " + TypeToString<T>(), "Loaded Content: " + name);

			return t;
		}

		static const T* Get(const std::string& name)
		{
			auto it = loadedContent.find(name);

			if (it == loadedContent.end())
			{
				throw std::exception("Content not loaded.");
				return nullptr;
			}

			return it->second;
		}

		static void UnLoad(const std::string& name)
		{
			auto it = loadedContent.find(name);

			if (it == loadedContent.end())
			{
				throw std::exception("Content not loaded.");
				return;
			}

			EventBus<ContentUnloadedEvent<T>>::Send(it->first, it->second);

			loadedContent.erase(it);

			delete it->second;
		}

		private:
		static std::map<std::string, T*> loadedContent;
		std::map<std::string, std::string> meta;
	};
}