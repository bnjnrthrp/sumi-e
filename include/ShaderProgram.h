#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <map>



using std::string;

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    bool loadShaders(const char* vsFilename, const char* fsFilename);
    void use();
    void setUniform(const GLchar* name, const glm::vec2& v);
    void setUniform(const GLchar* name, const glm::vec3& v);
    void setUniform(const GLchar* name, const glm::vec4& v);
    void setUniform(const GLchar* name, const glm::mat4& m);
    void setUniform(const GLchar* name, const GLfloat& f);
    void setUniformSampler(const GLchar* name, const GLint& slot);

    // Learn OpenGL uniform setters
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }



    GLuint getProgram()const;


private:
    string fileToString(const string& filename);
    void checkCompileErrors(GLuint shader, ShaderType type);

    GLuint m_ID;
    GLuint m_uniformModel;
    GLuint m_uniformProjection;

    std::map<string, GLint> m_UniformLocations;
    GLint getUniformLocation(const GLchar* name);
};


#endif // SHADER_PROGRAM_H

