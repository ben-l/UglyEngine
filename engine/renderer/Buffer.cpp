#include <uepch.h>
#include <Buffer.h>

#include <Renderer.h>
#include <OpenGLBuffer.h>

namespace Ugly {
     Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
     {
         switch(Renderer::GetAPI())
         {
             case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr; 
             case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
         }
         UE_CORE_ASSERT(false, "Unknown RendererAPI!");
         return nullptr;
         
     }

     Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
     {
         switch(Renderer::GetAPI())
         {
             case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr; 
             case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
         }
         UE_CORE_ASSERT(false, "Unknown RendererAPI!");
         return nullptr;
         
     }

     Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
     {
         switch(Renderer::GetAPI())
         {
             case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
             case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, size);
         }
         UE_CORE_ASSERT(false, "Unknown RendererAPI!");
         return nullptr;
     }
}
