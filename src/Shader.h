#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map> // a hash map



class Shader
{
private:
    // the program ID
    unsigned int m_RendererID;
    // caching for uniforms
    std::unordered_map<std::string, int> m_UniformLocationCache;

    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
    // check if uniform exist in the shader
    int GetUniformLocation(const std::string& name);
public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // Destructor 
    ~Shader();
    // use/activate the shader
    void use() const;
    // unuse / deactivate the shader
    void unuse() const;
    // utility uniform functions
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec3f(const std::string& name, glm::vec3 value);
    void setMat4f(const std::string& name, glm::mat4 value);
};


