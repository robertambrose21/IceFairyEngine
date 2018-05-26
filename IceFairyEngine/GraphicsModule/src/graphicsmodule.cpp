#include "graphicsmodule.h"

using namespace IceFairy;

GraphicsModule::GraphicsModule(const std::string& name)
    : Module(name),
	  hasErrors(false),
      windowWidth(0),
      windowHeight(0),
      window(NULL),
      title(""),
      sceneTree(std::shared_ptr<SceneTree>(new SceneTree()))
{ }

bool GraphicsModule::Initialise(void) {
	if (!Module::Initialise()) {
		return false;
	}

    glfwSetErrorCallback(GLFWErrorCallback);

    if (!glfwInit()) {
        hasErrors = true;
		Logger::PrintLn(Logger::LEVEL_ERROR, "\tFailed to initialise GLFW.");
        return false;
    }

    GLFWVersion glfwVersion = GetGLFWVersion();
    Logger::PrintLn(Logger::LEVEL_INFO, "\tGLFW %d.%d.%d Initialised OK.", glfwVersion.major, glfwVersion.revision, glfwVersion.revision);
	return true;
}

void GraphicsModule::StartMainLoop(void) {
    if (GetMainShader().GetProgramID() == 0) {
        throw MainShaderNotSetException();
    }

    if (drawables.empty()) {
        Logger::PrintLn(Logger::LEVEL_DEBUG, "No drawables have been created!");
    }

    screenQuad = CreateScreenQuad();

    while (!ShouldCloseLoop()) {
        glViewport(0, 0, windowWidth, windowHeight);

        std::map<std::string, GLuint> bufferData;

        for (auto& drawable : drawables) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            drawable->DrawBuffers(windowWidth, windowHeight);
            drawable->DrawFrame(mainShader);

            for (auto& bufferObject : drawable->GetBufferObjects()) {
                bufferData[bufferObject->GetName()] = bufferObject->GetTextureID();
            }
        }

        DrawBuffersToScreen(bufferData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GraphicsModule::DrawBuffersToScreen(std::map<std::string, GLuint>& bufferData) {
	GetMainShader().Bind();

    unsigned int i = 0;

    for (auto& data : bufferData) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, data.second);

        mainShader.SetUniform1i(data.first.c_str(), i++);
    }

    screenQuad->Draw();
}

VertexObject GraphicsModule::CreateScreenQuad(void) {
    unsigned int indices[6] = {
        0, 1, 2, 2, 3, 0
    };

    IceFairy::Vector3f vertices[4] = {
        IceFairy::Vector3f(1.0f, -1.0f, 0.0f),
        IceFairy::Vector3f(1.0f, 1.0f, 0.0f),
        IceFairy::Vector3f(-1.0f, 1.0f, 0.0f),
        IceFairy::Vector3f(-1.0f, -1.0f, 0.0f)
    };

    IceFairy::Vector3f normals[4] = {
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f)
    };

    IceFairy::Vector2f texcoords[4] = {
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f)
    };

    return CreateVertexObject(indices, 6, vertices, normals, texcoords, 4);
}

void GraphicsModule::AddDrawable(std::shared_ptr<Drawable> drawable) {
    drawables.push_back(drawable);
}

void GraphicsModule::AddDrawables(DrawablesList value) {
    this->drawables.insert(this->drawables.end(), value.begin(), value.end());
}

void GraphicsModule::CreateWindow(unsigned int ww, unsigned int wh, const char* winTitle, Colour3f backgroundColour) {
    if (!IsWindowCreated()) {
        this->window = CreateGLFWwindow(ww, wh, winTitle, NULL, NULL);

        if (IsWindowCreated()) {
            this->windowWidth = ww;
            this->windowHeight = wh;
            this->title = winTitle;

            glfwMakeContextCurrent(window);

            SetBackgroundColour(backgroundColour);
            EnableGLFlags();

            InitialiseGlew();
        }
        else {
            hasErrors = true;
            TerminateGLFW();
        }
    }
    else {
        throw MultipleWindowsUnsupportedException();
    }
}

void GraphicsModule::InitialiseGlew(void) {
    if (GLenum glewOK = glewInit() != GLEW_OK) {
        Logger::PrintLn(Logger::LEVEL_CRITICAL, "glew failed to initialise: %s", glewGetErrorString(glewOK));
        hasErrors = true;

        throw GLEWInitialisationFailureException();
    }
}

void GraphicsModule::EnableGLFlags(void) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0f);
}

void GraphicsModule::SetMainShader(const Shader& value) {
    this->mainShader = value;
}

Shader GraphicsModule::GetMainShader(void) {
    return mainShader;
}

void GraphicsModule::SetBackgroundColour(Colour3f backgroundColour) {
    glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 1.0f);
}

std::shared_ptr<SceneTree> GraphicsModule::GetSceneTree(void) {
    return sceneTree;
}

unsigned int GraphicsModule::GetWindowWidth(void) const {
    return windowWidth;
}

unsigned int GraphicsModule::GetWindowHeight(void) const {
    return windowHeight;
}

float GraphicsModule::GetAspectRatio(void) {
    return windowWidth / (float) windowHeight;
}

const char* GraphicsModule::GetWindowTitle(void) const {
    return title;
}

bool GraphicsModule::IsWindowCreated(void) {
    return window != NULL;
}

bool GraphicsModule::ShouldCloseLoop(void) {
    return glfwWindowShouldClose(window) || hasErrors;
}

// TODO: Why the fuck is this a method
GLFWwindow* GraphicsModule::CreateGLFWwindow(int width, int height, const char* winTitle, GLFWmonitor* monitor, GLFWwindow* share) {
    return glfwCreateWindow(width, height, winTitle, monitor, share);
}

void GraphicsModule::TerminateGLFW(void) {
    glfwTerminate();
}

GraphicsModule::GLFWVersion GraphicsModule::GetGLFWVersion(void) {
    GLFWVersion glfwVersion;
    glfwGetVersion(&glfwVersion.major, &glfwVersion.minor, &glfwVersion.revision);
    return glfwVersion;
}

void GraphicsModule::GLFWErrorCallback(int error, const char* description) {
    Logger::PrintLn(Logger::LEVEL_ERROR, "A GLFW error has occurred: [%d] %s", error, description);
}