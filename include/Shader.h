#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;
    // Constructor builds the shader from source strings.
    Shader(const char* vertexSource, const char* fragmentSource);
    // Activate the shader program.
    void use();
    // Utility uniform functions.
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
};

#endif
