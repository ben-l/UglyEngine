#include "uepch.h"
#include "Shader.h"
#include "Renderer.h"
#include "OpenGLShader.h"

namespace Ugly {

    Ref<Shader> Shader::Create(const std::string& filepath){
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
        }
        UE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
        UE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader){
        UE_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader){
        auto& name = shader->GetName();
        Add(name, shader);
    }

    Ugly::Ref<Ugly::Shader> ShaderLibrary::Load(const std::string& filepath){
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ugly::Ref<Ugly::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ugly::Ref<Ugly::Shader> ShaderLibrary::Get(const std::string& name){
        UE_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const 
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}
