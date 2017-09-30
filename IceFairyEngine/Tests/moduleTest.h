#ifndef __module_test_h__
#define __module_test_h__

#include <memory>
#include <string>
#include "gtest\gtest.h"
#include "gmock\gmock.h"

#include "core\module.h"
#include "modules\modulemapper.h"

const char* SUB_MODULE_A = "Module A";
const char* SUB_MODULE_B = "Module B";
const char* SUB_MODULE_C = "Module C";
const char* INVALID_MODULE = "Invalid Module";
const char* INVALID_SUB_MODULE = "Invalid SubModule";

class ModuleTest : public ::testing::Test {
    virtual void SetUp();
    virtual void TearDown();
};

class MockModule : public IceFairy::Module {
public:
    class TestSubModule : public IceFairy::Module {
    public:
        TestSubModule(std::string name) : IceFairy::Module(name){ }
        virtual ~TestSubModule() { }
        void Initialise(void) { }
    };

    MockModule(std::string name);

    std::shared_ptr<IceFairy::Module>   CreateSubModule(std::string subModuleName);
    void                                Initialise(void);

    MOCK_METHOD1(CreateSubModuleProxy, IceFairy::Module* (std::string subModuleName));
};

class InvalidSubModule : public IceFairy::Module {
public:
    InvalidSubModule();
    virtual ~InvalidSubModule();

    void Initialise(void);
};

#endif /* __module_test_h__ */