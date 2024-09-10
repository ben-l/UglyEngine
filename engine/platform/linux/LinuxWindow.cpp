#include <uepch.h>
#include <LinuxWindow.h>
#include <ApplicationEvent.h>
#include <MouseEvent.h>
#include <KeyEvent.h>
#include <OpenGLContext.h>

namespace Ugly {

    static uint8_t s_GLFWWindowCount = 0;
    
    static bool s_GLFWInitialized = false;
    static void GLFWErrorCallback(int error, const char* description){
        UE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props){
        return new LinuxWindow(props);
    }

 
    LinuxWindow::LinuxWindow(const WindowProps& props)
    {
        UE_PROFILE_FUNCTION();
        Init(props);
    }
     
     
    LinuxWindow::~LinuxWindow()
    {
        UE_PROFILE_FUNCTION();
        Shutdown();
    }

    void LinuxWindow::Init(const WindowProps& props){

        UE_PROFILE_FUNCTION();

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;


        UE_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width,
                     props.Height);


        if(!s_GLFWInitialized){
            //TODO: glfwTerminate on sys shutdown
            int success = glfwInit();
            UE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

		#if defined(__linux__)
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, 
                                    m_Data.Title.c_str(), nullptr, nullptr);

        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();
        // ^
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW Callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, 
                    int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            UE_CORE_WARN("{0}, {1}", width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key,
                    int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action){
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,
                    int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action){
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset,
                    double yOffset)
                    
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);

        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window,
                    double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
    }

    void LinuxWindow::Shutdown(){
        UE_PROFILE_FUNCTION();
        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0){
            glfwTerminate();
        }
    }

    void LinuxWindow::OnUpdate(){
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void LinuxWindow::SetVSync(bool enabled){
        UE_PROFILE_FUNCTION();

        if(enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool LinuxWindow::IsVSync() const {
        return m_Data.VSync;
    }
}
