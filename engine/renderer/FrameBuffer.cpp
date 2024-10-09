#include "uepch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "OpenGLFrameBuffer.h"

namespace Ugly {
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec){
         switch(Renderer::GetAPI())
         {
             case RendererAPI::API::None: UE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported"); return nullptr; 
             case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
         }
         UE_CORE_ASSERT(false, "Unknown RendererAPI!");
         return nullptr;
    }

};
