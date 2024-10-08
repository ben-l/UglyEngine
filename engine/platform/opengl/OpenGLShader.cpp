#include <uepch.h>
#include <OpenGLShader.h>

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
 
namespace Ugly {
    static GLenum ShaderTypeFromString(const std::string& type){
        if (type == "vertex"){
            return GL_VERTEX_SHADER;
        }
        if (type == "fragment" || type == "pixel"){
            return GL_FRAGMENT_SHADER;
        }
        UE_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        UE_PROFILE_FUNCTION();

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        //  extract filename from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        // rfind will find exact name
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
        UE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(sources);
    }
     
     
    OpenGLShader::~OpenGLShader()
    {
        UE_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath){
        UE_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in){
            in.seekg(0, std::ios::end); // go to end of file
            result.resize(in.tellg()); // tell us where ptr is, which is at the end (sizeof file)
            in.seekg(0, std::ios::beg); // go back to start of file
            in.read(&result[0], result.size());
            in.close();
        }
        else {
            UE_CORE_ERROR("Could not open file '{0}'", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        UE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken); // #type is 5 chars
        size_t pos = source.find(typeToken, 0); // find matching string of '#type'
                                                //
        while (pos != std::string::npos) {
            size_t eol = source.find_first_of("\r\n", pos); // find new line after #type and provide position
            UE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; // advance 5 chars across, then + 1 is space which brings us to
                                                      // the type 'vertex' etc.
            std::string type = source.substr(begin, eol - begin); // extract the type
            // check if type is accepted
            UE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified"); 
            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = 
                source.substr(nextLinePos,
                        pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        }
        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources){

        UE_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
        UE_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders atm");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;

        for (auto& kv: shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

		    // Create an empty vertex shader handle
		    GLuint shader = glCreateShader(type);

		    // Send the vertex shader source code to GL
		    // Note that std::string's .c_str is NULL character terminated.
		    const GLchar* sourceCStr = source.c_str();
		    glShaderSource(shader, 1, &sourceCStr, 0);
		    
		    // Compile the vertex shader
		    glCompileShader(shader);
		    
		    GLint isCompiled = 0;
		    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		    if(isCompiled == GL_FALSE)
		    {
		    	GLint maxLength = 0;
		    	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		    
		    	// The maxLength includes the NULL character
		    	std::vector<GLchar> infoLog(maxLength);
		    	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
		    	
		    	// We don't need the shader anymore.
		    	glDeleteShader(shader);
		    
		    	// Use the infoLog as you see fit.
                UE_CORE_ERROR("{0}", infoLog.data());
		        UE_CORE_ASSERT(false, "Shader compilation failure!");	
                break;
		    }
		    // Attach our shaders to our program
		    glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
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
			
			// Delete program as its no longer required
			glDeleteProgram(program);

            for (auto id : glShaderIDs)
			    glDeleteShader(id);

			
            UE_CORE_ERROR("{0}", infoLog.data());
		    UE_CORE_ASSERT(false, "Shader link failure!");	
            return;
		}
		// Always detach shaders after a successful link.
        for (auto id : glShaderIDs){
		    glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_RendererID = program;
		
    }


    void OpenGLShader::Bind() const
    {
        UE_PROFILE_FUNCTION();

        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        UE_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        UE_PROFILE_FUNCTION();

        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count){
        UploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        UE_PROFILE_FUNCTION();

        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        UE_PROFILE_FUNCTION();

        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        UE_PROFILE_FUNCTION();

        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        UE_PROFILE_FUNCTION();

        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count){
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

}
