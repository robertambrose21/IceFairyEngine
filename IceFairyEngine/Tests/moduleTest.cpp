#include "moduleTest.h"

using ::testing::Return;
using ::testing::_;
using namespace IceFairy;

void ModuleTest::SetUp() {
    IceFairy::Logger::EnableLogging(false);
}

void ModuleTest::TearDown() {
    IceFairy::Logger::EnableLogging(true);
}

MockModule::MockModule(std::string name)
    : Module(name)
{
    ON_CALL(*this, CreateSubModuleProxy(SUB_MODULE_A)).WillByDefault(Return(new TestSubModule(SUB_MODULE_A)));
    ON_CALL(*this, CreateSubModuleProxy(SUB_MODULE_B)).WillByDefault(Return(new TestSubModule(SUB_MODULE_B)));
    ON_CALL(*this, CreateSubModuleProxy(SUB_MODULE_C)).WillByDefault(Return(new TestSubModule(SUB_MODULE_C)));
}

std::shared_ptr<IceFairy::Module> MockModule::CreateSubModule(std::string subModuleName) {
    return std::shared_ptr<IceFairy::Module>(CreateSubModuleProxy(subModuleName));
}

void MockModule::Initialise(void) {
    AddSubModule(SUB_MODULE_A);
    AddSubModule(SUB_MODULE_B);
    AddSubModule(SUB_MODULE_C);
}

InvalidSubModule::InvalidSubModule() 
    : Module(INVALID_MODULE)
{ }

InvalidSubModule::~InvalidSubModule() {
}

void InvalidSubModule::Initialise(void) {
    AddSubModule(INVALID_SUB_MODULE);
}

/////////////////////////////////////////////////////////////////
////////////////////////// BEGIN TESTS //////////////////////////
/////////////////////////////////////////////////////////////////
TEST_F(ModuleTest, ListSubModules) {
    MockModule module("Test Module");
    
    EXPECT_CALL(module, CreateSubModuleProxy(_)).Times(3);

    module.Initialise();

    std::string expected = "[" +
        std::string(SUB_MODULE_A) + ", " + 
        std::string(SUB_MODULE_B) + ", " + 
        std::string(SUB_MODULE_C) + "]";

    ASSERT_EQ(module.ListSubModules(), expected);
}

TEST_F(ModuleTest, InitialiseNoSuchModule) {
    InvalidSubModule module;

    EXPECT_THROW(module.Initialise(), NoSuchModuleException);
}