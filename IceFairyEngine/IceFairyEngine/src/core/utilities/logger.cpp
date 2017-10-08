#include "logger.h"

using namespace IceFairy;

Logger::Logger()
    : logStream(&std::cout),
      loggingEnabled(true),
      htmlOpenTag("<div class=\"log-level-%d\">%s</div><div class=\"timestamp\">%s</div><div class=\"log-message\">"),
      htmlCloseTag("</div>"),
      logLevel(LEVEL_INFO)
{ }

std::string Logger::GetTimestamp(void) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct std::tm local;
    localtime_s(&local, &now);

    std::stringstream ss;
    ss << std::put_time(&local, "%d/%m/%y %H:%M:%S");

    return ss.str();
}

void Logger::EnableLogging(bool loggingEnabled) {
    GetInstance()._EnableLogging(loggingEnabled);
}

void Logger::_EnableLogging(bool loggingEnabled) {
    this->loggingEnabled = loggingEnabled;
}

bool Logger::IsLoggingEnabled(void)  {
    return GetInstance()._IsLoggingEnabled();
}

bool Logger::_IsLoggingEnabled(void) const {
    return loggingEnabled;
}

std::ostream* Logger::GetLogStream(void) {
    return Logger::GetInstance()._GetLogStream();
}

std::ostream* Logger::_GetLogStream(void) {
    return logStream;
}

unsigned int Logger::GetLogLevel(void) {
    return Logger::GetInstance()._GetLogLevel();
}

unsigned int Logger::_GetLogLevel(void) const {
    return logLevel;
}

std::string Logger::GetHTMLOpenTag(void) {
    return Logger::GetInstance()._GetHTMLOpenTag();
}

std::string Logger::_GetHTMLOpenTag(void) const {
    return htmlOpenTag;
}

std::string Logger::GetHTMLCloseTag(void) {
    return Logger::GetInstance()._GetHTMLCloseTag();
}

std::string Logger::_GetHTMLCloseTag(void) const {
    return htmlCloseTag;
}

void Logger::SetLogLevel(unsigned int logLevel) {
    Logger::GetInstance()._SetLogLevel(logLevel);
}

void Logger::_SetLogLevel(unsigned int logLevel) {
    this->logLevel = logLevel;
}

std::string Logger::GetLogLevelText(unsigned int level) {
    return Logger::GetInstance()._GetLogLevelText(level);
}

std::string Logger::_GetLogLevelText(unsigned int level) {
    switch (level) {
    case LEVEL_DEBUG:
        return "DEBUG";
    case LEVEL_INFO:
        return "INFO";
    case LEVEL_WARNING:
        return "WARNING";
    case LEVEL_ERROR:
        return "ERROR";
    case LEVEL_CRITICAL:
        return "CRITICAL";
    default:
        return "";
    }
}

void Logger::SetHTMLTags(const std::string& htmlOpenTag, const std::string& htmlCloseTag) {
    Logger::GetInstance()._SetHTMLTags(htmlOpenTag, htmlCloseTag);
}

void Logger::_SetHTMLTags(const std::string& htmlOpenTag, const std::string& htmlCloseTag) {
    this->htmlOpenTag = htmlOpenTag;
    this->htmlCloseTag = htmlCloseTag;
}

void Logger::SetLogStream(std::ostream& logStream) {
    Logger::GetInstance()._SetLogStream(logStream);
}

void Logger::_SetLogStream(std::ostream& logStream) {
    this->logStream = &logStream;
}

void Logger::Print(const char* fmt, ...) {
    ICEFAIRY_LOGGER_PRINT(fmt);                          
}

void Logger::Print(const std::string& fmt, ...) {
    const char* fmt_cstr = fmt.c_str();
    ICEFAIRY_LOGGER_PRINT(fmt_cstr);
}

void Logger::PrintL(const char* fmt, unsigned int bufferSize, ...) {
    ICEFAIRY_LOGGER_PRINT_LEN(fmt, bufferSize);
}

void Logger::PrintL(const std::string& fmt, unsigned int bufferSize, ...) {
    const char* fmt_cstr = fmt.c_str();
    ICEFAIRY_LOGGER_PRINT_LEN(fmt_cstr, bufferSize);
}

void Logger::PrintLn(unsigned int logLevel, const char* fmt, ...) {
    if (logLevel >= GetLogLevel()) {
        Print("[%s] %s: ", GetLogLevelText(logLevel).c_str(), GetTimestamp().c_str());
        ICEFAIRY_LOGGER_PRINT(fmt);
        Print("\n");
    }
}

void Logger::Print(unsigned int logLevel, const char* fmt, ...) {
	if (logLevel >= GetLogLevel()) {
		Print("[%s] %s: ", GetLogLevelText(logLevel).c_str(), GetTimestamp().c_str());
		ICEFAIRY_LOGGER_PRINT(fmt);
		Print("\n");
	}
}

void Logger::PrintLn(unsigned int logLevel, std::string fmt, ...) {
    if (logLevel >= GetLogLevel()) {
        Print("[%s] %s: ", GetLogLevelText(logLevel).c_str(), GetTimestamp().c_str());
        const char* fmt_cstr = fmt.c_str();
        ICEFAIRY_LOGGER_PRINT(fmt_cstr);
        Print("\n");
    }
}

void Logger::Print(unsigned int logLevel, std::string fmt, ...) {
	if (logLevel >= GetLogLevel()) {
		Print("[%s] %s: ", GetLogLevelText(logLevel).c_str(), GetTimestamp().c_str());
		const char* fmt_cstr = fmt.c_str();
		ICEFAIRY_LOGGER_PRINT(fmt_cstr);
		Print("\n");
	}
}

void Logger::PrintHTML(unsigned int level, const char* fmt, ...) {
    if (level >= GetLogLevel()) {
        Print(GetHTMLOpenTag().c_str(), level, GetLogLevelText(level).c_str(), GetTimestamp().c_str());
        ICEFAIRY_LOGGER_PRINT(fmt);
        Print((GetHTMLCloseTag() + '\n').c_str());
    }
}

void Logger::PrintHTML(unsigned int level, const std::string& fmt, ...) {
    if (level >= GetLogLevel()) {
        Print(GetHTMLOpenTag().c_str(), level, GetLogLevelText(level).c_str(), GetTimestamp().c_str());
        const char* fmt_cstr = fmt.c_str();
        ICEFAIRY_LOGGER_PRINT(fmt_cstr);
        Print((GetHTMLCloseTag() + '\n').c_str());
    }
}