#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
// constructor reads and builds the shader
{
    // 1. retrieve the vertex/fragment source code from filePath ------------------------------------------
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders --------------------------------------------------------------------------------
    unsigned int vertex, fragment;    // m_RendererID for vertex shader, and fragment shader
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL); // attach the shader source code to the shader object
    glCompileShader(vertex); // compile the shader
    // check if compilation was successful after the call to glCompileShader()
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL); // attach the shader source code to the shader object
    glCompileShader(fragment); // compile the shader
    // check if compilation was successful after the call to glCompileShader()
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    m_RendererID = glCreateProgram(); // shader Program ID
    glAttachShader(m_RendererID, vertex);
    glAttachShader(m_RendererID, fragment);
    glLinkProgram(m_RendererID);
    // print linking errors if any
    checkCompileErrors(m_RendererID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::use() const
// use/activate the shader
{
    glUseProgram(m_RendererID);
}

void Shader::unuse() const
// unuse/deactivate the shader
{
    glUseProgram(0);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
// utility function for checking shader compilation/linking errors.
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}


// utility uniform functions
void Shader::setBool(const std::string& name, bool value)
// set uniform boolean
{
    glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value)
// set uniform int
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value)
// set uniform float
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::setVec3f(const std::string& name, glm::vec3 value)
// set uniform matrix4f
{
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::setMat4f(const std::string& name, glm::mat4 value)
// set uniform matrix4f
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)
// check if uniform exists in the shader
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) // For performance boost
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning! uniform " << name << " does not exist." << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}
