#include "Log.h"
#include "Renderer/Shader.h"

#include "glm/gtc/type_ptr.hpp"
#include <filesystem>
#include <fstream>

namespace Ambient
{
    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        std::string source = ReadFile(filepath);
        auto shaders = Preprocess(source);
        Compile(shaders);

        std::filesystem::path fspath = filepath;
        m_Name = fspath.stem().string();
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource,
            const std::string& fragmentSource)
            : m_Name(name)
    {
        std::unordered_map<GLenum, std::string> shaders;
        shaders[GL_VERTEX_SHADER] = vertexSource;
        shaders[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(shaders);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetFloat3(const std::string& name, glm::vec3 vector)
    {
        UploadUniformFloat3(name, vector);
    }

    void OpenGLShader::SetFloat4(const std::string& name, glm::vec4 vector)
    {
        UploadUniformFloat4(name, vector);
    }

    void OpenGLShader::SetMat3(const std::string& name, glm::mat3 matrix)
    {
        UploadUniformMat3(name, matrix);
    }

    void OpenGLShader::SetMat4(const std::string& name, glm::mat4 matrix)
    {
        UploadUniformMat4(name, matrix);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1d(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, vector.x, vector.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());

        // 1 = No. of matrices
        // GL_FALSE = no need to transpose. But for DirectX (which uses row-major
        // ordering), transposing is needed
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());

        // 1 = No. of matrices
        // GL_FALSE = no need to transpose. But for DirectX (which uses row-major
        // ordering), transposing is needed
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            AM_CORE_ERROR("Could not open file {}", filepath);
        }

        return result;
    }

    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
        {
            return GL_VERTEX_SHADER;
        }
        else if (type == "fragment" || type == "pixel")
        {
            return GL_FRAGMENT_SHADER;
        }

        AM_CORE_ERROR("Unknown shader type: {}", type);

        return 0;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaders;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);

            if (eol == std::string::npos)
            {
                AM_CORE_ERROR("Syntax Error");
            }

            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);

            if (!ShaderTypeFromString(type))
            {
                AM_CORE_ERROR("Invalid Shader Type: {}", type);
            }

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaders[ShaderTypeFromString(type)] =
                    source.substr(nextLinePos,
                            pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        }

        return shaders;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders)
    {
        // Source: https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language

        GLuint program = glCreateProgram();

        // Only support 2 shaders for now
        if (shaders.size() > 2)
        {
            AM_CORE_ERROR("More than 2 shaders in one file are not supported");
        }
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDsIndex = 0;

        for (auto& i : shaders)
        {
            GLenum type = i.first;
            const std::string& sourceString = i.second;

            // Create an empty shader handle
            GLuint shader = glCreateShader(type);

            // Send the shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* source = (const GLchar*)sourceString.c_str();
            glShaderSource(shader, 1, &source, 0);

            // Compile the shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                AM_CORE_ERROR("{} shader compilation failure! {}", type, infoLog.data());
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDsIndex++] = shader;
        }

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for (auto shaderID : glShaderIDs)
            {
                glDeleteShader(shaderID);
            }

            AM_CORE_ERROR("Shader Linking failure! {}", infoLog.data());
            return;
        }

        // Always detach shaders after a successful link.
        for (auto shaderID : glShaderIDs)
        {
            glDetachShader(program, shaderID);
        }

        m_RendererID = program;
    }

} // namespace Ambient
