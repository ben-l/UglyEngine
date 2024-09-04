#include <uepch.h>
#include <Buffer.h>

#include <Renderer.h>
#include <OpenGLBuffer.h>

namespace Ugly {
     VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
     {
         switch(Renderer::GetAPI())
         {
             case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr; 
             case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
         }
         UE_CORE_ASSERT(false, "Unknown RendererAPI!");
         return nullptr;
         
     }

     IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
     {
         switch(Renderer::GetAPI())
         {
             case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr;
             case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
         }
         UE_CORE_ASSERT(false, "Unknown RendererAPI!");
         return nullptr;
     }
}
