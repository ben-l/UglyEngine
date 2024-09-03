#pragma once
#include <GraphicsContext.h>

struct GLFWwindow;

namespace UglyEngine {
    class OpenGLContext : public GraphicsContext {
        public:
            OpenGLContext(GLFWwindow* windowHandle);

            virtual void Init() override;
            virtual void SwapBuffers() override;

        private:
            GLFWwindow* m_WindowHandle;
    };
}
