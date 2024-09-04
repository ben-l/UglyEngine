#include <uepch.h>
#include <RenderCommand.h>
#include <OpenGLRendererAPI.h>

namespace Ugly {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
