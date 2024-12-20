#pragma once

#include <Core.h>

#include <Window.h>
#include <LayerStack.h>
#include <Event.h>
#include <ApplicationEvent.h>

#include <Timestep.h>

#include <ImGuiLayer.h>

namespace Ugly {
    class Application
    {
        public:
            Application(const std::string& name = "Ugly App");
            virtual ~Application() = default;

            void Run();

            void OnEvent(Event& e);

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);

            Window& GetWindow() { return *m_Window; }

            void Close();

            ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

            static Application& Get() { return *s_Instance; }
        private:
            bool OnWindowClose(WindowCloseEvent& e);
            bool OnWindowResize(WindowResizeEvent& e);
        private:
            std::unique_ptr<Window> m_Window;
            ImGuiLayer* m_ImGuiLayer;
            bool m_Running = true;
            bool m_Minimized = false;
            LayerStack m_LayerStack;
            float m_LastFrameTime = 0.0f;
        private:
            static Application* s_Instance;
    };
    // to be defined in client (sandboxapp)
    Application* CreateApplication();
}
