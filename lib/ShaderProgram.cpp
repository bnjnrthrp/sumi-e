#include "../include/ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>


ShaderProgram::ShaderProgram()
    : m_ID(0),
      m_uniformModel(0),
      m_uniformProjection(0)
{}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_ID);
}


bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
    string vsString = fileToString(vsFilename);
    string fsString = fileToString(fsFilename);
    const GLchar* vsSourcePtr = vsString.c_str();
    const GLchar* fsSourcePtr = fsString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(vs, 1, &vsSourcePtr, NULL);
    glShaderSource(fs, 1, &fsSourcePtr, NULL);

    glCompileShader(vs);
    checkCompileErrors(vs, VERTEX);

    glCompileShader(fs);
    checkCompileErrors(fs, FRAGMENT);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);

    checkCompileErrors(m_ID, PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(fs);

    m_UniformLocations.clear();
    return true;
}
void ShaderProgram::use()
{
    if (m_ID > 0)
        glUseProgram(m_ID);
}

string ShaderProgram::fileToString(const string& filename)
{
    std::stringstream ss;
    std::ifstream file;

    try
    {
        file.open(filename, std::ios::in);

        if (!file.fail())
        {
            ss << file.rdbuf();
        }

        file.close();
    }
    catch (std::exception ex)
    {
        std::cerr << "Error reading shader filename!" << std::endl;
    }

    return ss.str();
}
void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
    int status = 0;

    if (type == PROGRAM)
    {
        glGetProgramiv(m_ID, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetProgramInfoLog(m_ID, length, &length, &errorLog[0]);
            std::cerr << "Error! Program failed to link. " << errorLog << std::endl;
        }
    }
    else // VERTEX or FRAGMENT
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
        }
    }

}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
    std::map<string, GLint>::iterator it = m_UniformLocations.find(name);

    if (it == m_UniformLocations.end())
    {
        m_UniformLocations[name] = glGetUniformLocation(m_ID, name);
    }

    return m_UniformLocations[name];
}

GLuint ShaderProgram::getProgram()const
{
    return m_ID;
}


void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* name, const GLfloat& f)
{
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, f);
}
void ShaderProgram::setUniformSampler(const GLchar* name, const GLint& slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, slot);
}