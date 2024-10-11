#pragma once
#include <Layer.h>
#include <ApplicationEvent.h>
#include <KeyEvent.h>
#include <MouseEvent.h>

namespace Ugly {
    class UGLY_ENGINE_API ImGuiLayer : public Layer {
        public:
            ImGuiLayer();
            ~ImGuiLayer();

            virtual void OnAttach() override;
            virtual void OnDetach() override;


            void OnEvent(Event& e);
            void Begin();
            void End();

            void BlockEvents(bool block) { m_BlockEvents = block; }

        private:
            float m_Time = 0.0f;
            bool m_BlockEvents = true;

    };
}


