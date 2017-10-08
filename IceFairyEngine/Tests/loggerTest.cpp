#include "loggerTest.h"

void LoggerTest::SetUp() {
    srand((unsigned int)time(NULL));
    IceFairy::Logger::SetLogStream(logStringStream);
}

void LoggerTest::TearDown() {
    IceFairy::Logger::SetLogStream(std::cout);
}

std::stringstream& LoggerTest::GetLogStream(void) {
    return logStringStream;
}

std::string LoggerTest::GetCurrentStreamOutput(void) {
    return GetLogStream().str();
}

void LoggerTest::ClearStream(void) {
    GetLogStream().str(std::string());
    GetLogStream().clear();
}

std::string LoggerTest::GenerateRandomB64String(unsigned int length) {
    std::string b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    std::string result;

    for (unsigned int i = 0; i < length; i++) {
        result += b64[rand() % (b64.length() - 1)];
    }

    return result;
}

/////////////////////////////////////////////////////////////////
////////////////////////// BEGIN TESTS //////////////////////////
/////////////////////////////////////////////////////////////////
TEST_F(LoggerTest, MultiplePrint) {
    IceFairy::Logger::Print("%s %s\n", "This is", "a test string");
    EXPECT_EQ("This is a test string\n", GetCurrentStreamOutput());
    IceFairy::Logger::Print("%s: %d\n", "Here\'s a number", 12);
    EXPECT_EQ("This is a test string\nHere\'s a number: 12\n", GetCurrentStreamOutput());
    ClearStream();
}

TEST_F(LoggerTest, SetLogStream) {
    std::stringstream tempStream;

    IceFairy::Logger::Print("%s", TEST_STRING_A);
    IceFairy::Logger::SetLogStream(tempStream);
    IceFairy::Logger::Print("%s", TEST_STRING_B);
    IceFairy::Logger::SetLogStream(GetLogStream());
    IceFairy::Logger::Print("%s", TEST_STRING_B);

    EXPECT_EQ(std::string(TEST_STRING_A) + std::string(TEST_STRING_B), GetCurrentStreamOutput());
    EXPECT_EQ(TEST_STRING_B, tempStream.str());

    ClearStream();

    EXPECT_EQ("", GetCurrentStreamOutput());
}

TEST_F(LoggerTest, LongFormatName) {
    std::string fmt = GenerateRandomB64String(ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH);
    std::string text = GenerateRandomB64String(ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH);

    EXPECT_THROW(IceFairy::Logger::Print(fmt, text), IceFairy::PrintBufferTooSmallException);
    EXPECT_THROW(IceFairy::Logger::PrintL(fmt, ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH, text), IceFairy::PrintBufferTooSmallException);
    EXPECT_NO_THROW(IceFairy::Logger::PrintL(fmt, ICEFAIRY_LOGGER_DEFAULT_CHAR_LENGTH + 1, text));

    ClearStream();
}

TEST_F(LoggerTest, LogLevel) {
    std::string infoStr = IceFairy::Logger::GetLogLevelText(IceFairy::Logger::LEVEL_INFO);
    std::string warningStr = IceFairy::Logger::GetLogLevelText(IceFairy::Logger::LEVEL_WARNING);
    std::string errorStr = IceFairy::Logger::GetLogLevelText(IceFairy::Logger::LEVEL_ERROR);

    std::string tt = IceFairy::Logger::GetTimestamp();

    IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_INFO, "%s\n", "Some information");
    EXPECT_NE(std::string::npos, GetLogStream().str().find(infoStr));

    ClearStream();

    IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_WARNING);
    IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_INFO, "%s\n", "Some more information");
    IceFairy::Logger::PrintLn(IceFairy::Logger::LEVEL_WARNING, "%s\n", "A warning");

    EXPECT_EQ(std::string::npos, GetLogStream().str().find(infoStr));
    EXPECT_NE(std::string::npos, GetLogStream().str().find(warningStr));

    ClearStream();
}

TEST_F(LoggerTest, HTMLOutput) {
    std::string openTag = "<div class=\"log-level-%d\">%s</div><div class=\"timestamp\">%s</div><div class=\"log-message\">";
    std::string closeTag = "</div>";

    IceFairy::Logger::SetLogLevel(IceFairy::Logger::LEVEL_DEBUG);
    IceFairy::Logger::SetHTMLTags(openTag, closeTag);
    IceFairy::Logger::PrintHTML(IceFairy::Logger::LEVEL_INFO, "%s", "Some information");

    std::string logString = GetLogStream().str();
    unsigned int len = logString.length();

    EXPECT_NE(std::string::npos, logString.find("<div class=\"log-level-1\">INFO</div><div class=\"timestamp\">"));
    EXPECT_EQ(closeTag, logString.substr(len - closeTag.length() - 1, closeTag.length()));

    ClearStream();
}

TEST_F(LoggerTest, EnableLogging) {
    std::string message = "A test message";

    IceFairy::Logger::EnableLogging(false);
    IceFairy::Logger::Print(message);

    EXPECT_EQ("", GetLogStream().str());

    IceFairy::Logger::EnableLogging(true);
    IceFairy::Logger::Print(message);

    EXPECT_EQ(message, GetLogStream().str());

    ClearStream();
}