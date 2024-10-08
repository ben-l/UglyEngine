#include <uepch.h>
#include <OpenGLContext.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Ugly {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
        : m_WindowHandle(windowHandle)
    {
        UE_CORE_ASSERT(windowHandle, "Handle is null");
    }

    void OpenGLContext::Init()
    {
        UE_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        UE_CORE_ASSERT(status, "Failed to initialize Glad!");
        UE_CORE_INFO("OpenGL Info:");
        UE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        UE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        UE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        UE_PROFILE_FUNCTION();
        glfwSwapBuffers(m_WindowHandle);
    }

}
