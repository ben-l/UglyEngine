#pragma once
#include <Layer.h>

#include <ApplicationEvent.h>
#include <KeyEvent.h>
#include <MouseEvent.h>

namespace Ugly {
    class UGLY_ENGINE_API ImGuiLayer : public Layer {
        public:
            ImGuiLayer();
            ~ImGuiLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnEvent(Event& e) override;

            void Begin();
            void End();

            void BlockEvents(bool block) { m_BlockEvents = block; }
            void SetDarkThemeColors();
        private:
            bool m_BlockEvents = true;
            float m_Time = 0.0f;

    };
}


