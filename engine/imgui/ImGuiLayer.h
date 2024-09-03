#pragma once
#include <Layer.h>
#include <ApplicationEvent.h>
#include <KeyEvent.h>
#include <MouseEvent.h>

namespace UglyEngine {
    class UGLY_ENGINE_API ImGuiLayer : public Layer {
        public:
            ImGuiLayer();
            ~ImGuiLayer();

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnImGuiRender() override;

            void Begin();
            void End();

        private:
            float m_Time = 0.0f;

    };
}


