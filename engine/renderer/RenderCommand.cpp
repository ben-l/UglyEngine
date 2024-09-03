#include <uepch.h>
#include <RenderCommand.h>
#include <OpenGLRendererAPI.h>

namespace UglyEngine {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
