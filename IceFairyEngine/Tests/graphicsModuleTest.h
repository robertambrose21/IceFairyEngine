#ifndef __graphics_module_test__
#define __graphics_module_test__

#include "gtest\gtest.h"
#include "gmock\gmock.h"

#include "modules\graphicsmodule.h"
#include "modules\modulemapper.h"
#include "math\colour.h"
#include "graphics\drawable.h"
#include "graphics\shader.h"

class GraphicsModuleTest : public ::testing::Test {
    virtual void SetUp();
    virtual void TearDown();
};

class MockGraphicsModule : public IceFairy::GraphicsModule {
public:
    MockGraphicsModule();

    MOCK_METHOD1(SetBackgroundColour, void(IceFairy::Colour3f backgroundColour));
    MOCK_METHOD5(CreateGLFWwindow, GLFWwindow*(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share));
    MOCK_METHOD0(TerminateGLFW, void());
    MOCK_METHOD0(ShouldCloseLoop, bool());
    MOCK_METHOD0(GetMainShader, IceFairy::Shader());

private:
    void    InitialiseGlew(void) { }
};

class TestDrawable : public IceFairy::Drawable {
public:
    TestDrawable(MockGraphicsModule& module, bool stopLoop);

    void DrawFrame(IceFairy::Shader shader, std::shared_ptr<IceFairy::BufferObject> bufferObject);

    MOCK_METHOD0(DrawTheFrameMock, void());

private:
    MockGraphicsModule& module;
    bool                stopLoop;
};

#endif /* __graphics_module_test__ */