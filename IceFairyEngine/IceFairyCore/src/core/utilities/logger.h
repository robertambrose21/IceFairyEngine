#ifndef __logger_h__
#define __logger_h__

#include <memory>
#include <iostream>
#include <string>
#include <cstdarg>
#include <exception>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "icexception.h"

#define ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH 1024
/*! \internal */
#define ICEFAIRY_LOGGER_PRINT_LEN(fmt, len) {       \
    if(!IsLoggingEnabled())                         \
        return;                                     \
    else if(GetLogStream() == nullptr)              \
        throw InvalidLogStreamException();          \
    char* text = new char[len];                     \
    if (fmt != NULL) {                              \
        va_list ap;                                 \
        va_start(ap, fmt);                          \
        if (_vscprintf(fmt, ap) >= (signed) len) {  \
            delete[] text;                          \
            throw PrintBufferTooSmallException();   \
        }                                           \
        vsnprintf_s(text, len, len, fmt, ap);       \
        va_end(ap);                                 \
    }                                               \
    *(GetLogStream()) << text;                      \
    delete[] text;                                  \
}
/*! \def ICEFAIRY_LOGGER_PRINT
 * Print information to the logger see Print functions for further details.
 */
#define ICEFAIRY_LOGGER_PRINT(fmt) ICEFAIRY_LOGGER_PRINT_LEN(fmt, ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH)

#pragma warning(disable : 4505)

namespace IceFairy {
	/*! \brief Thrown when the print buffer is too small. */
	class PrintBufferTooSmallException : public ICException {
	public:
		/*! \internal */
		PrintBufferTooSmallException()
			: ICException("The buffer size supplied for Print is too small, try a larger buffer.") {
		}
	};

	/*! \brief Thrown when the log stream is invalid. */
	class InvalidLogStreamException : public ICException {
	public:
		/*! \internal */
		InvalidLogStreamException()
			: ICException("The current log stream is invalid.") {
		}
	};

	/*! \brief Logging functionality. */
	class Logger {
	public:
		/*! \brief Logging level options */
		enum Level {
			//! Trace level logging
			LEVEL_TRACE,
			//! Debug level logging
			LEVEL_DEBUG,
			//! General information level logging
			LEVEL_INFO,
			//! Logging level for any warnings
			LEVEL_WARNING,
			//! Logging level for any errors
			LEVEL_ERROR,
			//! Logging level for any critical errors
			LEVEL_CRITICAL
		};

		/*! \brief Returns the current timestamp.
		 *
		 * \returns the current timestamp in the format of 'DD/MM/YYYY HH:MM:SS'.
		 */
		static std::string GetTimestamp(void);

		/*! \brief Enables or disables logging.
		 *
		 * \param enabled Whether or not logging should be enabled.
		 */
		static void EnableLogging(bool loggingEnabled);

		/*! \brief Returns whether logging is currently enabled.
		 *
		 * \returns Whether logging is currently enabled.
		 */
		static bool IsLoggingEnabled(void);

		/*! \brief Returns the current log stream.
		 *
		 * \returns The current log stream.
		 */
		static std::ostream* GetLogStream(void);

		/*! \brief Returns the current log level.
		 *
		 * \returns The current log level.
		 */
		static unsigned int GetLogLevel(void);

		/*! \brief Returns the current HTML open tag.
		 *
		 * \returns The current HTML open tag.
		 */
		static std::string GetHTMLOpenTag(void);

		/*! \brief Returns the current HTML close tag.
		*
		* \returns The current HTML close tag.
		*/
		static std::string GetHTMLCloseTag(void);

		/*! \brief Sets the lowest \ref Level of logging that should be printed.
		 *
		 * \param level The log \ref Level to set
		 */
		static void SetLogLevel(unsigned int logLevel);

		/*! \brief Returns a given log \ref Level as text.
		*
		* \param level The log \ref Level to convert to a string.
		* \returns The log \ref Level as a string. If no appropriate level is found an empty string is returned instead.
		*/
		static std::string GetLogLevelText(unsigned int level);

		/*! \brief Sets the HTML tags to use for the \ref PrintHTML functions.
		 *
		 * The supplied open/close tags wrap the message passed to the print function.
		 * Three parameters are required for the following arguments (in order):
		 * - %d: /ref Level enum value.
		 * - %s: /ref Level text value.
		 * - %s: Timestamp.
		 *
		 * The default HTML tags are constructed as follows:
		 * \code{.cpp}
		 * std::string openTag = "<div class=\"log-level-%d\">%s</div><div class=\"timestamp\">%s</div><div class=\"log-message\">";
		 * std::string closeTag = "</div>";
		 *
		 * IceFairy::Logger::SetHTMLTags(openTag, closeTag);
		 * \endcode
		 *
		 * This will create the following string when printing the message "Hello" with a log \ref Level of ::WARNING:
		 * \code{.html}
		 * <div class="log-level-2">WARNING</div><div class="timestamp">20/12/2015 01:10:23</div><div class="log-message">Hello</div>
		 * \endcode
		 * \param openTag The opening HTML tag
		 * \param closeTag The closing HTML tag
		 */
		static void SetHTMLTags(const std::string& htmlOpenTag, const std::string& htmlCloseTag);

		/*! \brief Sets the current log stream.
		 *
		 * Sets the current log output stream for the global \c Print functions.\n
		 * The default output stream is \c std::cout
		 * \param stream The stream for the logger to output to.
		 */
		static void SetLogStream(std::ostream& logStream);

		/*! \brief Prints to the current log stream. <tt>const char*</tt> variant
		 *
		 * Prints to the current log stream using \c printf notation.\n
		 * The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		 * If you require a larger print buffer please refer to the other Print functions.
		 * \note This function will ignore any \ref Level restrictions and will not generate a timestamp.
		 *
		 * \code{.cpp}
		 * IceFairy::Logger:Print("%s: %d\n", "The number is", 7);
		 * \endcode
		 * \param fmt The format of the string to print.
		 * \param ... The arguments for the formatted string to print.
		 * \throws PrintBufferTooSmallException
		 * \throws InvalidLogStreamException
		 */
		static void Print(const char* fmt, ...);

		/*! \brief Prints to the current log stream. \c std::string variant.
		 *
		 * Prints to the current log stream using \c printf notation.\n
		 * The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		 * If you require a larger print buffer please refer to the other Print functions.
		 * \note This function will ignore any \ref Level restrictions and will not generate a timestamp.
		 *
		 * \code{.cpp}
		 * IceFairy::Logger:Print("%s: %d\n", "The number is", 7);
		 * \endcode
		 * \param fmt The format of the string to print.
		 * \param ... The arguments for the formatted string to print.
		 * \throws PrintBufferTooSmallException
		 * \throws InvalidLogStreamException
		 */
		static void Print(const std::string& fmt, ...);

		/*! \brief Prints to the current log stream. <tt>const char*</tt> variant.
		*
		* Prints to the current log stream using \c printf notation.\n
		* Additionally the print buffer size may be specified here. This should only be used
		* if the default buffer size (\ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH) is not sufficient.
		* \note This function will ignore any \ref Level restrictions and will not generate a timestamp.
		*
		* \code{.cpp}
		* IceFairy::Logger:Print("%s: %d\n", "The number is", 7);
		* \endcode
		* \param fmt The format of the string to print.
		* \param bufferSize The size of the print buffer.
		* \param ... The arguments for the formatted string to print.
		* \throws PrintBufferTooSmallException
		* \throws InvalidLogStreamException
		*/
		static void PrintL(const char* fmt, unsigned int bufferSize, ...);

		/*! \brief Prints to the current log stream. \c std::string variant.
		 *
		 * Prints to the current log stream using \c printf notation.\n
		 * Additionally the print buffer size may be specified here. This should only be used
		 * if the default buffer size (\ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH) is not sufficient.
		 * \note This function will ignore any \ref Level restrictions and will not generate a timestamp.
		 *
		 * \code{.cpp}
		 * IceFairy::Logger:Print("%s: %d\n", "The number is", 7);
		 * \endcode
		 * \param fmt The format of the string to print.
		 * \param bufferSize The size of the print buffer.
		 * \param ... The arguments for the formatted string to print.
		 * \throws PrintBufferTooSmallException
		 * \throws InvalidLogStreamException
		 */
		static void PrintL(const std::string& fmt, unsigned int bufferSize, ...);

		/*! \brief Prints to the current log stream with a given log level and timestamp and a new line. <tt>const char*</tt> variant
		*
		* Prints to the current log stream using \c printf notation.\n
		* The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		* If you require a larger print buffer please refer to the other Print functions.
		* \note This function adds a newline at the end of the stream.
		*
		* \code{.cpp}
		* IceFairy::Logger:Print(IceFairy::Logger::INFO, "%s\n", "This is a message with some info");
		* \endcode
		* \param level The level to log this message at. See \ref Level.
		* \param fmt The format of the string to print.
		* \param ... The arguments for the formatted string to print.
		* \throws PrintBufferTooSmallException
		* \throws InvalidLogStreamException
		*/
		static void PrintLn(unsigned int logLevel, const char* fmt, ...);

		/*! \brief Prints to the current log stream with a given log level and timestamp. <tt>const char*</tt> variant
		*
		* Prints to the current log stream using \c printf notation.\n
		* The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		* If you require a larger print buffer please refer to the other Print functions.
		* \note This function adds a newline at the end of the stream.
		*
		* \code{.cpp}
		* IceFairy::Logger:Print(IceFairy::Logger::INFO, "%s\n", "This is a message with some info");
		* \endcode
		* \param level The level to log this message at. See \ref Level.
		* \param fmt The format of the string to print.
		* \param ... The arguments for the formatted string to print.
		* \throws PrintBufferTooSmallException
		* \throws InvalidLogStreamException
		*/
		static void Print(unsigned int logLevel, const char* fmt, ...);

		/*! \brief Prints to the current log stream with a given log level and timestamp and a new line. \c std::string variant
		 *
		 * Prints to the current log stream using \c printf notation.\n
		 * The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		 * If you require a larger print buffer please refer to the other Print functions.
		 * \note This function adds a newline at the end of the stream.
		 *
		 * \code{.cpp}
		 * IceFairy::Logger:Print(IceFairy::Logger::INFO, "%s\n", "This is a message with some info");
		 * \endcode
		 * \param level The level to log this message at. See \ref Level.
		 * \param fmt The format of the string to print.
		 * \param ... The arguments for the formatted string to print.
		 * \throws PrintBufferTooSmallException
		 * \throws InvalidLogStreamException
		 */
		static void PrintLn(unsigned int logLevel, std::string fmt, ...);

		/*! \brief Prints to the current log stream with a given log level and timestamp. \c std::string variant
		*
		* Prints to the current log stream using \c printf notation.\n
		* The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		* If you require a larger print buffer please refer to the other Print functions.
		* \note This function adds a newline at the end of the stream.
		*
		* \code{.cpp}
		* IceFairy::Logger:Print(IceFairy::Logger::INFO, "%s\n", "This is a message with some info");
		* \endcode
		* \param level The level to log this message at. See \ref Level.
		* \param fmt The format of the string to print.
		* \param ... The arguments for the formatted string to print.
		* \throws PrintBufferTooSmallException
		* \throws InvalidLogStreamException
		*/
		static void Print(unsigned int logLevel, std::string fmt, ...);

		/*! \brief Prints to the current log stream with a given log level and timestamp in HTML format. <tt>const char*</tt> variant
		 *
		 * Prints to the current log stream using \c printf notation.\n
		 * The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		 * If you require a larger print buffer please refer to the other Print functions.
		 * \note This function adds a newline at the end of the stream.
		 *
		 * \code{.cpp}
		 * IceFairy::Logger:Print(IceFairy::Logger::INFO, "%s\n", "This is a message with some info");
		 * \endcode
		 * \param level The level to log this message at. See \ref Level.
		 * \param fmt The format of the string to print.
		 * \param ... The arguments for the formatted string to print.
		 * \throws PrintBufferTooSmallException
		 * \throws InvalidLogStreamException
		 */
		static void PrintHTML(unsigned int level, const char* fmt, ...);

		/*! \brief Prints to the current log stream with a given log level and timestamp in HTML format. <tt>const char*</tt> variant
		 *
		 * Prints to the current log stream using \c printf notation.\n
		 * The default maximum length for the print buffer is defined by \ref ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH.\n
		 * If you require a larger print buffer please refer to the other Print functions.
		 * \note This function adds a newline at the end of the stream.
		 *
		 * \code{.cpp}
		 * IceFairy::Logger:Print(IceFairy::Logger::INFO, "%s\n", "This is a message with some info");
		 * \endcode
		 * \param level The level to log this message at. See \ref Level.
		 * \param fmt The format of the string to print.
		 * \param ... The arguments for the formatted string to print.
		 * \throws PrintBufferTooSmallException
		 * \throws InvalidLogStreamException
		 */
		static void PrintHTML(unsigned int level, const std::string& fmt, ...);

	private:
		static Logger& GetInstance() {
			static Logger instance;
			return instance;
		}

		Logger();
		~Logger() { }

		Logger(Logger const&) = delete;
		void operator=(Logger const&) = delete;

		void            _SetLogStream(std::ostream& logStream);
		void            _SetHTMLTags(const std::string& htmlOpenTag, const std::string& htmlCloseTag);
		std::string     _GetLogLevelText(unsigned int level);
		void            _SetLogLevel(unsigned int logLevel);
		std::string     _GetHTMLOpenTag(void) const;
		unsigned int    _GetLogLevel(void) const;
		std::ostream* _GetLogStream(void);
		bool            _IsLoggingEnabled(void) const;
		void            _EnableLogging(bool loggingEnabled);
		std::string     _GetHTMLCloseTag(void) const;

		std::ostream* logStream;
		bool            loggingEnabled;
		std::string     htmlOpenTag;
		std::string     htmlCloseTag;
		unsigned int    logLevel;
	};
}

#endif /* __logger_h__ */