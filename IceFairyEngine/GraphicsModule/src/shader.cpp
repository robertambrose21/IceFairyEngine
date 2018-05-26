#include "shader.h"

using namespace IceFairy;

Shader::Shader()
    : programID(0),
      modelMatrixName(""),
      viewProjectionMatrixName("")
{ }

Shader::Shader(const std::string& vertexFilename, const std::string& fragmentFilename)
    : Shader()
{
    AttachShaderFromFile(vertexFilename, GL_VERTEX_SHADER);
    AttachShaderFromFile(fragmentFilename, GL_FRAGMENT_SHADER);

    Link();
    Validate();
}

void Shader::SetUniform1f(const char* name, float val) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform1f(loc, val);
}

void Shader::SetUniform2f(const char* name, float a, float b) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform2f(loc, a, b);
}

void Shader::SetUniform2f(const char* name, Vector2f vec) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform2f(loc, vec.x, vec.y);
}

void Shader::SetUniform3f(const char* name, float a, float b, float c) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform3f(loc, a, b, c);
}

void Shader::SetUniform3f(const char* name, Vector3f vec) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::SetUniform3f(const char* name, Colour3f colour) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform3f(loc, colour.r, colour.g, colour.b);
}

void Shader::SetUniform4f(const char* name, float a, float b, float c, float d) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform4f(loc, a, b, c, d);
}

void Shader::SetUniform4f(const char* name, Colour4f colour) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform4f(loc, colour.r, colour.g, colour.b, colour.a);
}

void Shader::SetUniform3fv(const char* name, GLsizei count, const GLfloat* value) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform3fv(loc, count, value);
}

void Shader::SetUniform1f(std::string name, float val) {
    SetUniform1f(name.c_str(), val);
}

void Shader::SetUniform2f(std::string name, float a, float b) {
    SetUniform2f(name.c_str(), a, b);
}

void Shader::SetUniform2f(std::string name, Vector2f vec) {
    SetUniform2f(name.c_str(), vec);
}

void Shader::SetUniform3f(std::string name, float a, float b, float c) {
    SetUniform3f(name.c_str(), a, b, c);
}

void Shader::SetUniform3f(std::string name, Vector3f vec) {
    SetUniform3f(name.c_str(), vec);
}

void Shader::SetUniform3f(std::string name, Colour3f colour) {
    SetUniform3f(name.c_str(), colour);
}

void Shader::SetUniform4f(std::string name, float a, float b, float c, float d) {
    SetUniform4f(name.c_str(), a, b, c, d);
}

void Shader::SetUniform4f(std::string name, Colour4f colour) {
    SetUniform4f(name.c_str(), colour);
}

void Shader::SetUniform3fv(std::string name, GLsizei count, const GLfloat* value) {
    SetUniform3fv(name.c_str(), count, value);
}

void Shader::SetUniform1i(const char* name, int val) {
    GLint loc = glGetUniformLocation(programID, name);
    glUniform1i(loc, val);
}

void Shader::SetUniform2i(const char* name, int a, int b) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform2i(loc, a, b);
}

void Shader::SetUniform3i(const char* name, int a, int b, int c) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform3i(loc, a, b, c);
}

void Shader::SetUniform4i(const char* name, int a, int b, int c, int d) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniform4i(loc, a, b, c, d);
}

void Shader::SetUniform1i(std::string name, int val) {
    SetUniform1i(name.c_str(), val);
}

void Shader::SetUniform2i(std::string name, int a, int b) {
    SetUniform2i(name.c_str(), a, b);
}

void Shader::SetUniform3i(std::string name, int a, int b, int c) {
    SetUniform3i(name.c_str(), a, b, c);
}

void Shader::SetUniform4i(std::string name, int a, int b, int c, int d) {
    SetUniform4i(name.c_str(), a, b, c, d);
}

void Shader::SetUniformMatrixfv(const char* name, const Matrix4f& matrix) {
    GLuint loc = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix.Val(0, 0));
}

void Shader::SetUniformMatrixfv(std::string name, const Matrix4f& matrix) {
    SetUniformMatrixfv(name.c_str(), matrix);
}

void Shader::SetModelMatrixName(const std::string& value) {
    this->modelMatrixName = value;
}

void Shader::SetViewProjectionMatrixName(const std::string& value) {
    this->viewProjectionMatrixName = value;
}

void Shader::SetModelMatrix(const Matrix4f& matrix) {
    if (modelMatrixName != "")
        SetUniformMatrixfv(modelMatrixName.c_str(), matrix);
    else
        throw ShaderMatrixNameNotSetException();
}

void Shader::SetViewProjectionMatrix(const Matrix4f& matrix) {
    if (viewProjectionMatrixName != "")
        SetUniformMatrixfv(viewProjectionMatrixName.c_str(), matrix);
    else
        throw ShaderMatrixNameNotSetException();
}

void Shader::AttachShaderFromFile(const std::string& filename, GLenum shaderType) {
    AttachShader(LoadSourceFromFile(filename), shaderType);
}

void Shader::AttachShader(std::string shaderSource, GLenum shaderType) {
    if (programID == 0)
        programID = glCreateProgram();

    const GLchar *sp[1];
    GLint lengths[1];

    GLuint shader = glCreateShader(shaderType);

    shaderSource = ScanIncludes(shaderSource);

    sp[0] = shaderSource.c_str();
    lengths[0] = shaderSource.length();

    glShaderSource(shader, 1, sp, lengths);
    glCompileShader(shader);

    glAttachShader(programID, shader);

    CheckForErrors(shader, shaderType);
}

void Shader::Link(void) {
    glLinkProgram(programID);
}

void Shader::Validate(void) {
    glValidateProgram(programID);
}

void Shader::Bind(void) {
    glUseProgram(programID);
}

void Shader::Unbind(void) {
    glUseProgram(0);
}

GLuint Shader::GetProgramID(void) const {
    return programID;
}

void Shader::CheckForErrors(GLuint shaderID, GLenum shaderType) {
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);

        Logger::PrintLn(Logger::LEVEL_ERROR, "Compilation error in shader \'%s\': %s\n",
            shaderType == GL_VERTEX_SHADER ? VERTEX_SHADER_NAME : FRAGMENT_SHADER_NAME, strInfoLog);

        delete[] strInfoLog;

        throw ShaderCompilationException();
    }
}

std::string Shader::LoadSourceFromFile(const std::string& filename) {
    return Resource(filename).GetData();
}

std::string Shader::ScanIncludes(const std::string& shaderData) {
    std::string buffer;
    std::stringstream ss;
    ss << shaderData;

    while (ss.good()) {
        int streampos = (int)ss.tellg();

        std::getline(ss, buffer);

        const unsigned int iLength = std::string("#include").length();
        if (buffer.substr(0, iLength) == "#include") {
            std::string includeFile = RemoveCharacters(buffer.substr(iLength + 1, buffer.length()), "\"\n\r\t");

            std::string data = Resource("shaders/" + includeFile).GetData();

            std::string temp(ss.str());
            temp.replace(streampos, buffer.length(), data);
            ss.str(temp);
        }
    }

    return ss.str();
}

std::string Shader::RemoveCharacters(std::string str, const std::string& characters) {
    for (unsigned int i = 0; i < characters.length(); i++) {
        str.erase(std::remove(str.begin(), str.end(), characters.at(i)), str.end());
    }

    return str;
}