#ifndef __ice_fairy_shader_h__
#define __ice_fairy_shader_h__

#include <string>
#include <sstream>
#include <algorithm>

#include "glinclude.h"
#include "math\vector.h"
#include "math\matrix.h"
#include "math\colour.h"
#include "core\utilities\logger.h"
#include "core\utilities\resource.h"
#include "core\utilities\icexception.h"

namespace IceFairy {
    class ShaderCompilationException : public ICException {
    public:
        /*! \internal */
        ShaderCompilationException()
            : ICException("An error occurred compiling a shader.")
        { }
    };

    class ShaderMatrixNameNotSetException : public ICException {
    public:
        /*! \internal */
        ShaderMatrixNameNotSetException()
            : ICException("The matrix name has not been set for this shader.")
        { }
    };

    /*! \brief Shader class for GLSL shaders.
     *
     * This class creates a shader program based off vertex and fragment shader sources.
     * Bind and Unbind activate the shader for use and disable it, respectively.\n
     * Currently, to use this shaderprogram, the shader in question but have a mat4 property
     * by the name of 'gWVP' for the push/pop functionality to work.
     * Sample usage:
     * \code{.cpp}
     * Shader program("shader.vert", "shader.frag");
     * program.Link();
     * program.Bind();
     * program.SetUniform3f("colour", Colour3(1.0f, 0.0f, 1.0f));
     * \endcode
     */
    class Shader {
    public:
        /*! \brief Creates the program. */
        Shader();
        /*! \brief Creates the program from given vertex/fragment shader files.
         *
         * \param vertexFilename The filename for the vertex shader
         * \param fragmentFilename The filename for the fragment shader
         */
        Shader(const std::string& vertexFilename, const std::string& fragmentFilename);

        // These functions send various uniform float properties to the shader with a given name.
        void		SetUniform1f(const char* name, float val);
        void		SetUniform2f(const char* name, float a, float b);
        void		SetUniform2f(const char* name, Vector2f vec);
        void		SetUniform3f(const char* name, float a, float b, float c);
        void		SetUniform3f(const char* name, Vector3f vec);
        void		SetUniform3f(const char* name, Colour3f colour);
        void		SetUniform4f(const char* name, float a, float b, float c, float d);
        void		SetUniform4f(const char* name, Colour4f colour);
        void		SetUniform3fv(const char* name, GLsizei count, const GLfloat* value);
        void		SetUniform1f(std::string name, float val);
        void		SetUniform2f(std::string name, float a, float b);
        void		SetUniform2f(std::string name, Vector2f vec);
        void		SetUniform3f(std::string name, float a, float b, float c);
        void		SetUniform3f(std::string name, Vector3f vec);
        void		SetUniform3f(std::string name, Colour3f colour);
        void		SetUniform4f(std::string name, float a, float b, float c, float d);
        void		SetUniform4f(std::string name, Colour4f colour);
        void		SetUniform3fv(std::string name, GLsizei count, const GLfloat* value);
        // Similarly, these functions send uniform integer properties to the shader with a given name.
        void		SetUniform1i(const char* name, int val);
        void		SetUniform2i(const char* name, int a, int b);
        void		SetUniform3i(const char* name, int a, int b, int c);
        void		SetUniform4i(const char* name, int a, int b, int c, int d);
        void		SetUniform1i(std::string name, int val);
        void		SetUniform2i(std::string name, int a, int b);
        void		SetUniform3i(std::string name, int a, int b, int c);
        void		SetUniform4i(std::string name, int a, int b, int c, int d);
        // Sends a uniform matrix to the shader with a given name.
        void		SetUniformMatrixfv(const char* name, const Matrix4f& matrix);
        void		SetUniformMatrixfv(std::string name, const Matrix4f& matrix);

        /*! \brief Sets the model matrix shader hook.
         *
         * \param modelMatrixName The name of the model matrix parameter in the shader.
         */
        void        SetModelMatrixName(const std::string& modelMatrixName);
        /*! \brief Sets the view proejction matrix shader hook.
        *
        * \param viewProjectionMatrixName The name of the view projection matrix parameter in the shader.
        */
        void        SetViewProjectionMatrixName(const std::string& viewProjectionMatrixName);

        /*! \brief Sets the model matrix for this shader.
         *
         * \param matrix The matrix to set.
         */
        void        SetModelMatrix(const Matrix4f& matrix);
        /*! \brief Sets the model matrix for this shader.
        *
        * \param matrix The matrix to set.
        */
        void        SetViewProjectionMatrix(const Matrix4f& matrix);

        /*! \brief Attach an aditional shader to this program with the given source.
         *
         * \param shaderSource The source for the shader.
         * \param shaderType The type of shader, options are \ref GL_VERTEX_SHADER or \ref GL_FRAGMENT_SHADER.
         */
        void		AttachShader(std::string shaderSource, GLenum shaderType);
        /*! \brief Attach an aditional shader to this program with the given file name.
         *
         * \param filename The filename for the shader.
         * \param shaderType The type of shader, options are \ref GL_VERTEX_SHADER or \ref GL_FRAGMENT_SHADER.
         */
        void		AttachShaderFromFile(const std::string& filename, GLenum shaderType);
        /*! \brief Links the program object with a call to glLinkProgram. */
        void		Link(void);
        /*! \brief Validates the program object with a call to glValidateProgram. */
        void        Validate(void);
        /*! \brief Sets this shader for use by a call to glUseProgram(programID) */
        void		Bind(void);
        /*! \brief 'Unbinds' this shader, i.e calls glUseProgram(0) */
        void		Unbind(void);

        /*! \returns the programs ID */
        GLuint		GetProgramID(void) const;

    private:
        void		CheckForErrors(GLuint shaderID, GLenum shaderType);
        std::string LoadSourceFromFile(const std::string& filename);
        std::string	ScanIncludes(const std::string& shaderData);

        std::string RemoveCharacters(std::string str, const std::string& characters);

        const char* FRAGMENT_SHADER_NAME = "Fragment Shader";
        const char* VERTEX_SHADER_NAME = "Vertex Shader";

        GLuint      programID;
        std::string modelMatrixName;
        std::string viewProjectionMatrixName;
    };
}

#endif /* __ice_fairy_shader_h__ */
