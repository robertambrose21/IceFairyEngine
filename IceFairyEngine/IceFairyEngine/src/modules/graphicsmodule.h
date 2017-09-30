#ifndef __ice_fairy_graphics_module_h__
#define __ice_fairy_graphics_module_h__

#include <vector>
#include <functional>

#include "../graphics/glinclude.h"
#include "../graphics/drawable.h"
#include "../core/module.h"
#include "../core/utilities/icexception.h"
#include "../math/colour.h"
#include "../graphics/shader.h"
#include "../graphics/scenetree.h"
#include "../graphics/bufferobject.h"
#include "modulenames.h"

namespace IceFairy {
    /*! \brief Thrown if the main shader has not been set */
    class MainShaderNotSetException : public ICException {
    public:
        /*! \internal */
        MainShaderNotSetException()
            : ICException("The main shader has not been set.")
        { }
    };

    /*! \brief Thrown if more than one window is created */
    class MultipleWindowsUnsupportedException : public ICException {
    public:
        /*! \internal */
        MultipleWindowsUnsupportedException()
            : ICException("Creating multiple windows is currently unsupported by this module.")
        { }
    };

    /*! \brief Thrown if more than one window is created */
    class GLEWInitialisationFailureException : public ICException {
    public:
        /*! \internal */
        GLEWInitialisationFailureException()
            : ICException("GLEW failed to initialise. See log for details.")
        { }
    };

    /*! \brief Graphics module for executing OpenGL functionality.
     *
     * Currently, only one window may be created via the \ref CreateWindow function.
     * This module expects objects instantiated from the \ref Drawable interface
     * to be loaded in prior to calling \ref StartMainLoop.
     * An example of combining an \ref Application and a \ref Drawable follows:
     * \code{.cpp}
     * class MyDrawable :
     *     public IceFairy::Application,
     *     public IceFairy::Drawable,
     *     public std::enable_shared_from_this<DemoApplication>
     * {
     * public:
     *     MyDrawable(int argc, char** argv);
     *     ~MyDrawable();
     *
     *     void Initialise(void) {
     *         module = std::static_pointer_cast<IceFairy::GraphicsModule>(LoadModule(ICEFAIRY_GRAPHICS_MODULE));
     *     }
     *
     *     void Start(void) {
     *         module->CreateWindow(1024, 768, "New App");
     *         module->SetMainShader(IceFairy::Shader("shaders/deferred.vert", "shaders/deferred.frag"));
     *         module->AddDrawable(shared_from_this());
     *         module->StartMainLoop();
     *     }
     *
     *     void DrawFrame(IceFairy::Shader shader, std::shared_ptr<IceFairy::BufferObject> bufferObject) {
     *         // Graphics code here...
     *     }
     * }
     * \endcode
     */
    class GraphicsModule : public Module {
    public:
        /*! \brief Constructor. */
        GraphicsModule();
        /*! \brief Destructor. */
        virtual ~GraphicsModule() { }

        /*! \brief Initialises OpenGL */
        void                        Initialise(void);
        /*! \brief Begins the main graphics loop, executing any drawables present
         *
         * \throws ColourShaderNotSetException
         */
        void                        StartMainLoop(void);
        /*! \brief Adds a single drawable to the graphics module.
         *
         * \param drawable The drawable to add.
         */
        void                        AddDrawable(std::shared_ptr<Drawable> drawable);
        /*! \brief Adds a list of drawables to the graphics module.
         *
         * \param drawables The list of drawables to add.
         */
        void                        AddDrawables(DrawablesList drawables);
        /*! \brief Creates an OpenGL window with a specified width, height, title and optional background colour.
         *
         * \param windowWidth The width of the window
         * \param windowHeight The height of the window
         * \param title The title of the window
         * \param backgroundColour The background colour of the window. Black by default.
         * \throws MultipleWindowsUnsupportedException
         */
        void                        CreateWindow(unsigned int windowWidth, unsigned int windowHeight, const char* title,
                                        Colour3f backgroundColour = Colour3f(0.0f, 0.0f, 0.0f));
        /*! \brief Sets the main \ref Shader
         *
         * \param mainShader The main \ref Shader for the GraphicsModule
         */
        void                        SetMainShader(const Shader& mainShader);
        /*! \returns The main \ref Shader used by the GraphicsModule */
        Shader                      GetMainShader(void);
        /*! \brief Sets the background colour of the window.
         *
         * \param backgroundColour The background colour to set.
         */
        virtual void                SetBackgroundColour(Colour3f backgroundColour);

        /*! \returns The width of the window. */
        unsigned int                GetWindowWidth(void) const;
        /*! \returns The height of the window. */
        unsigned int                GetWindowHeight(void) const;
        /*! \returns The aspect ratio of the window. */
        float                       GetAspectRatio(void);
        /*! \returns The title of the window. */
        const char*                 GetWindowTitle(void) const;
        /*! \returns \c true if the window has been created. */
        virtual bool                IsWindowCreated(void);
        
        /*! \returns The \ref SceneTree in use by this GraphicsModule */
        std::shared_ptr<SceneTree>  GetSceneTree(void);

    protected:
        typedef struct _GLFWVersion {
            int major;
            int minor;
            int revision;
        } GLFWVersion;

        GLFWVersion                 GetGLFWVersion(void);

        static void                 GLFWErrorCallback(int error, const char* description);
        virtual bool                ShouldCloseLoop(void);

        virtual GLFWwindow*         CreateGLFWwindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        virtual void                TerminateGLFW(void);

        virtual void                InitialiseGlew(void);
        void                        EnableGLFlags(void);
        VertexObject                CreateScreenQuad(void);

        void                        DrawBuffersToScreen(std::map<std::string, GLuint>& bufferData);

        GLFWwindow*                     window;
        unsigned int                    windowWidth;
        unsigned int                    windowHeight;
        const char*                     title;
        bool                            hasErrors;

        std::shared_ptr<SceneTree>      sceneTree;
        DrawablesList                   drawables;
        Shader                          mainShader;

        VertexObject                    screenQuad;
    };
}

#endif /* __ice_fairy_graphics_module_h__ */
