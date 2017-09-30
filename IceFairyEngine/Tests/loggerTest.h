#ifndef __logger_test_h__
#define __logger_test_h__

#include <sstream>

#include "gtest\gtest.h"
#include "core\utilities\logger.h"

class LoggerTest : public ::testing::Test {
protected:
    const char* TEST_STRING_A = "This is a test string";
    const char* TEST_STRING_B = "This is another test string";

    std::stringstream logStringStream;

    virtual void SetUp();
    virtual void TearDown();

    std::stringstream& GetLogStream(void);
    std::string GetCurrentStreamOutput(void);
    void ClearStream(void);
    std::string GenerateRandomB64String(unsigned int length);
};

#endif /* __logger_test_h__ */