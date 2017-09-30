#include "graphicsModuleTest.h"

using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::NiceMock;
using ::testing::_;
using namespace IceFairy;

void GraphicsModuleTest::SetUp() {
    IceFairy::Logger::EnableLogging(false);
}

void GraphicsModuleTest::TearDown() {
    IceFairy::Logger::EnableLogging(true);
}

MockGraphicsModule::MockGraphicsModule()
    : GraphicsModule()
{
    ON_CALL(*this, CreateGLFWwindow(300, 400, _, _, _)).WillByDefault(Return((GLFWwindow*) new int(1)));
    ON_CALL(*this, CreateGLFWwindow(0, 0, _, _, _)).WillByDefault(ReturnNull());
}

TestDrawable::TestDrawable(MockGraphicsModule& module, bool stopLoop)
    : module(module),
      stopLoop(stopLoop)
{ }

void TestDrawable::DrawFrame(IceFairy::Shader shader, std::shared_ptr<IceFairy::BufferObject> bufferObject) {
    ON_CALL(module, ShouldCloseLoop()).WillByDefault(Return(stopLoop));

    DrawTheFrameMock();
}

/////////////////////////////////////////////////////////////////
////////////////////////// BEGIN TESTS //////////////////////////
/////////////////////////////////////////////////////////////////
TEST_F(GraphicsModuleTest, CreateWindow) {
    NiceMock<MockGraphicsModule> module;

    EXPECT_CALL(module, SetBackgroundColour(Colour3f(0.5f, 0.5f, 0.5f))).Times(1);

    module.CreateWindow(300, 400, "Test Window", Colour3f(0.5f, 0.5f, 0.5f));

    EXPECT_EQ(300, module.GetWindowWidth());
    EXPECT_EQ(400, module.GetWindowHeight());
    EXPECT_EQ("Test Window", module.GetWindowTitle());
}

TEST_F(GraphicsModuleTest, CreateWindowMultiples) {
    NiceMock<MockGraphicsModule> module;

    module.CreateWindow(300, 400, "Test Window", Colour3f(0.5f, 0.5f, 0.5f));
    
    EXPECT_THROW(module.CreateWindow(100, 100, "Another Window"), MultipleWindowsUnsupportedException);
}

TEST_F(GraphicsModuleTest, CreateWindowBadParameters) {
    NiceMock<MockGraphicsModule> module;

    EXPECT_CALL(module, TerminateGLFW()).Times(1);

    module.CreateWindow(0, 0, "Bad Parameters");
}
//
//TEST_F(GraphicsModuleTest, StartMainLoop) {
//    NiceMock<MockGraphicsModule> module;
//
//    auto drawableA = std::shared_ptr<TestDrawable>(new TestDrawable(module, false));
//    auto drawableB = std::shared_ptr<TestDrawable>(new TestDrawable(module, false));
//    auto drawableC = std::shared_ptr<TestDrawable>(new TestDrawable(module, true));
//
//    IceFairy::DrawablesList drawables;
//    drawables.push_back(drawableB);
//    drawables.push_back(drawableC);
//
//    module.AddDrawable(drawableA);
//    module.AddDrawables(drawables);
//
//    EXPECT_CALL(*drawableA.get(), DrawTheFrameMock());
//    EXPECT_CALL(*drawableB.get(), DrawTheFrameMock());
//    EXPECT_CALL(*drawableC.get(), DrawTheFrameMock());
//
//    module.StartMainLoop();
//}