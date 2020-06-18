#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include "ValkyrieEngine/EventBus.hpp"

namespace vlk
{
	/*!
	 * \brief Defines various severities of log message
	 */
	enum class LogLevel
	{
		Trace,		/*!< Details regarding a step in a function or algorithm */
		Verbose,	/*!< Details regarding the output of a function or algorithm */
		Info,		/*!< Normal but significant conditions */
		Warning,	/*!< Abnormal conditions that could lead to an error */
		Error,		/*!< An error from which the program can easily recover */
		Fatal,		/*!< An error from which the program cannot recover */
		Announce	/*!< An announcement, always visible */
	};

	class LogEvent
	{
		public:
		constexpr LogEvent(const std::string_view& _message, const vlk::LogLevel _severity) :
			message(_message),
			severity(_severity)
		{
		}

		const std::string_view message;
		const vlk::LogLevel severity;
		//TODO: source location
	};
	
	/*!
	 * \brief Helper class that statically-evaluates whether log events with
	 * severity \link LogLevel \endlink should be sent
	 * \tparam L the \link LogLevel \endlink to evaluate
	 * \sa LogLevel
	 */
	template <LogLevel L>
	class IsLogLevelEnabled
	{
		static constexpr bool Test()
		{
			switch (L)
			{
				case LogLevel::Trace:
					return vlk::enable_trace_logging;
				case LogLevel::Verbose:
				case LogLevel::Info:
					return vlk::is_debug || vlk::enable_trace_logging;
				default:
					return true;
			}
		}
		public:
			
		/*!
		 * \brief Whether \link LogLevel \endlink L is enabled in the current configuration.
		 */
		static constexpr bool Value = Test();
	};
	
	/*!
	 * \brief Sends a log event 
	 */
	template <LogLevel L = LogLevel::Info>
	typename std::enable_if_t<IsLogLevelEnabled<L>::Value, void>
	Log(const std::string& message)
	{
		SendEvent(LogEvent(message, L));
	}
	
	template <LogLevel L = LogLevel::Info>
	typename std::enable_if_t<!IsLogLevelEnabled<L>::Value, void>
	Log(const std::string&) {}
}
