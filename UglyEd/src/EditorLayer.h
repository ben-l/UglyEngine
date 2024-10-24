#pragma once

#include "UglyEngine.h"
#include "ScenePanel.h"

namespace Ugly {
    class EditorLayer : public Layer {
        public:
            EditorLayer();
            virtual ~EditorLayer() = default;
            virtual void OnAttach() override;
            virtual void OnDetach() override;
            void OnUpdate(Timestep ts) override;
            virtual void OnImGuiRender() override;
            void OnEvent(Event& e) override;
        private:
    		OrthographicCameraController m_CameraController;
    
            // Temp
            Ref<VertexArray> m_SquareVA;
            Ref<Shader> m_FlatColorShader;
            Ref<FrameBuffer> m_FrameBuffer;

            Ref<Scene> m_ActiveScene;
            Entity m_SquareEntity;
            Entity m_CameraEntity;

            Entity m_SecondCamera;

            bool m_PrimaryCamera = true;
    
            Ref<Texture2d> m_CheckerboardTexture;
    
            glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        	bool m_ViewportFocused = false, m_ViewportHovered = false;
            glm::vec2 m_ViewportSize = {0.0f, 0.0f};

            ScenePanel m_Panel;
    
    };
};
