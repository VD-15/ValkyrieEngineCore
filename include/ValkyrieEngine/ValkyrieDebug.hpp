/*!
 * \file ValkyrieDebug.hpp
 * \brief Provides basic debugging utilities.
 */

#ifndef VLK_DEBUG_H
#define VLK_DEBUG_H

#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include "ValkyrieEngine/EventBus.hpp"
#include <type_traits>

namespace vlk
{
	/*!
	 * \enum LogLevel
	 * \brief Defines various severities of log message.
	 *
	 * Less severe log messages may not be processed depending on the configuration.
	 * \sa LogEvent
	 * \sa IsLogLevelEnabled
	 * \sa VLK_ENABLE_TRACE_LOGGING
	 */
	enum class LogLevel
	{
		Trace,		/*!< Details regarding a step in a function or algorithm */
		Verbose,	/*!< Details regarding the output of a function or algorithm */
		Info,		/*!< Normal but significant conditions */
		Warning,	/*!< Abnormal conditions that could lead to an error */
		Error,		/*!< An error from which the program can recover */
		Fatal,		/*!< An error from which the program cannot recover */
		Announce	/*!< An announcement, always visible */
	};

	/*!
	 * \brief An event that is sent whenever a message is logged.
	 *
	 * No event listeners for this are defined for log events by default. You may have to define your own in order to display these anywhere.
	 *
	 * \sa Log
	 * \sa LogLevel
	 * \sa EventBus
	 * \sa EventListener
	 */
	struct LogEvent
	{
		/*!
		 * \brief A description of what happened
		 */
		std::string message = "";

		/*!
		 * \brief How severe the event is
		 */
		LogLevel severity = LogLevel::Verbose;

		/*!
		 * \brief The file the message originated from
		 */
		std::string file = "";

		/*!
		 * \brief The line the message originated on
		 */
		Size line = 0;
	};
	
	/*!
	 * \brief Helper class that determines whether a specific vlk::LogLevel is enabled in the current configuration
	 *
	 * The following log levels are enabled in all configurations:
	 * - LogLevel::Warning
	 * - LogLevel::Error
	 * - LogLevel::Fatal
	 * - LogLevel::Announce
	 *
	 * In addition to those listed above, the following log levels are also enabled in debug configurations:
	 * - LogLevel::Info
	 *
	 * The <tt>VLK_ENABLE_TRACE_LOGGING</tt> macro can be defined to enable the following log levels:
	 * - LogLevel::Info
	 * - LogLevel::Verbose
	 *
	 * \tparam L the LogLevel to evaluate
	 * \sa Log(const std::string&)
	 * \sa VLK_IS_DEBUG
	 * \sa VLK_ENABLE_TRACE_LOGGING
	 */
	template <LogLevel L>
	class IsLogLevelEnabled
	{
		static inline VLK_CXX14_CONSTEXPR bool Test()
		{
			switch (L)
			{
				case LogLevel::Trace:
					return VLK_ENABLE_TRACE_LOGGING;
				case LogLevel::Verbose:
					VLK_FALLTHROUGH;
				case LogLevel::Info:
					return VLK_IS_DEBUG || VLK_ENABLE_TRACE_LOGGING;
				default:
					return true;
			}
		}
		public:
			
		/*!
		 * \brief <tt>true</tt> if log messages with a severity of <tt>L</tt> are processed in the current configuration.
		 */
		static VLK_CXX14_CONSTEXPR bool Value = Test();
	};

	/*!
	 * \brief Generates a log event
	 *
	 * \tparam L The severity of the event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	template <LogLevel L = LogLevel::Info, typename..., std::enable_if_t<IsLogLevelEnabled<L>::Value, Int> = 0>
	inline void Log(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		SendEvent(LogEvent{message, L, file, line});
	}

	/*!
	 * \brief Dummy function for disabled log events. Does nothing.
	 */
	template <LogLevel L = LogLevel::Info, typename..., std::enable_if_t<!IsLogLevelEnabled<L>::Value, Int> = 1>
	void Log(const std::string&, const std::string& = "", const Size = 0)
	{ }

	/*!
	 * \brief Generates a verbose log event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	inline void LogVerbose(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		Log<LogLevel::Verbose>(message, file, line);
	}

	/*!
	 * \brief Generates an info log event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	inline void LogInfo(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		Log<LogLevel::Info>(message, file, line);
	}

	/*!
	 * \brief Generates a warning log event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	inline void LogWarning(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		Log<LogLevel::Warning>(message, file, line);
	}

	/*!
	 * \brief Generates an error log event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	inline void LogError(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		Log<LogLevel::Error>(message, file, line);
	}

	/*!
	 * \brief Generates a fatal log event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	inline void LogFatal(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		Log<LogLevel::Fatal>(message, file, line);
	}

	/*!
	 * \brief Generates an announce log event
	 *
	 * \param message A description of what happened
	 * \param file The file the message originated from
	 * \param line The line the message originated on
	 */
	inline void LogAnnounce(const std::string& message, const std::string& file = "", const Size line = 0)
	{
		Log<LogLevel::Announce>(message, file, line);
	}

}

#endif
