/*!
 * \file ValkyrieDebug.hpp
 * \brief Provides basic debugging utilities.
 */

#ifndef VLK_DEBUG_H
#define VLK_DEBUG_H

#include "ValkyrieEngine/ValkyrieDefs.hpp"
#include "ValkyrieEngine/EventBus.hpp"

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
	 * \sa Log(const std::string&)
	 * \sa LogLevel
	 * \sa EventBus
	 * \sa EventListener
	 */
	class LogEvent
	{
		public:
		/*!
		 * \brief Constructs a log event with the given message and severity
		 * \param _message The message carried by the log event
		 * \param _severity The severity of the log event
		 * \param _file The source file that generated this log event
		 * \param _line The line of the source file that generated this log event
		 * \sa LogLevel 
		 * \sa Log(const std::string&)
		 * \todo constexpr std::strings are not currently supported by any stdlib implementation, update this to VLK_CXX20_CONSTEXPR when added
		 * \todo std::source_location is not surrently supported by any stdlib implementation, integrate this when added
		 */
		inline LogEvent(
			const std::string& _message, 
			const vlk::LogLevel _severity,
			const std::string& _file = "",
			const Size _line = 0) :
			message(_message),
			severity(_severity),
			file(_file),
			line(_line)
		{ }

		const std::string message; /*!< The message carried by this log event */
		const vlk::LogLevel severity; /*!< The severity of this log event */
		const std::string file; /*!< The source file that generated this log event */
		const Size line; /*!< The line of the source file that generated this log event */
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
	 * The <tt>#VLK_ENABLE_TRACE_LOGGING</tt> macro can be used to enable the following log levels:
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
	 * \brief Sends a log event with a given message
	 *
	 * Use this as the preferred method of sending debug messages in your application.
	 *
	 * \param message The message contained by the log event
	 * \param file The source file that generated this log event 
	 * \param line The line of the source file that generated this log event
	 * \tparam L The severity of this log event
	 */
	template <LogLevel L = LogLevel::Info>
	typename std::enable_if_t<IsLogLevelEnabled<L>::Value, void>
	Log(
		const std::string& message,
		const std::string& file = "",
		const Size line = 0)
	{
		SendEvent(LogEvent(message, L, file,  line));
	}
	
	/*!
	 * \brief Dummy function for disabled log events. Does nothing.
	 */
	template <LogLevel L = LogLevel::Info>
	typename std::enable_if_t<!IsLogLevelEnabled<L>::Value, void>
	Log(const std::string& message,
		const std::string& file = "",
		const Size line = 0) { }
}

#endif
