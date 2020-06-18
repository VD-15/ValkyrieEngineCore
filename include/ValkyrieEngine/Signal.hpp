#pragma once

#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include <map>
#include <any>
#include <optional>
#include <type_traits>

namespace vlk
{
	/*!
	 * \brief Represents a dynamic event sent to an entity,
	 * Unlike events, which are statically defined and sent globally via the EventBus, signals are
	 * completely dynamic and are sent only to the Entity that recieves them. The components that
	 * are attached to the entity then process the signal by inheriting from ISignalListener
	 */
	class Signal
	{
		std::map<std::string, std::any> args;

		public:
		const std::string type;

		Signal(std::string _type) :
			type(_type)
		{}

		/*!
		 * \brief Gets an argument from this signal
		 * \param argName The name of this argument
		 * \tparam T The type to cast the returned argument to
		 * \return an std::optional containing the requested argument or no value if the argument was not present or the cast failed
		 */
		template <typename T>
		std::optional<T> GetArg(std::string argName) const
		{
			if (auto it = args.find(argName); it != args.end())
			{
				try
				{
					return std::optional<T>(std::any_cast<T>(it->second));
				}
				catch (const std::bad_any_cast& ex)
				{
					return std::optional<T>();
				}
			}
			else
			{
				return std::optional<T>();
			}
		}

		/*!
		 * \brief Assigns an argument for this signal.
		 * If the argument is not present, it is inserted.
		 * If the argument is present, it is replaced.
		 * \param
		 */
		template <typename T>
		inline void PutArg(std::string argID, T val)
		{
			args[argID] = std::make_any<T>(val);
		}
	};

	/*!
	 * \brief Base interface for entity signal listeners
	 * \see SignalListener
	 */
	class ISignalListener
	{
		public:
		/*!
		 * \brief Sends a signal to this signal listener
		 * I don't know what it does, it's virtual.
		 */
		virtual void OnSignal(const Signal& signal) = 0;
	};
}
