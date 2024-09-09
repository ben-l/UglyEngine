#include "uepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "OpenGLTexture.h"

namespace Ugly {

    Ref<Texture2d> Texture2d::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
        }
        UE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2d> Texture2d::Create(const std::string& path){

        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }
        UE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
