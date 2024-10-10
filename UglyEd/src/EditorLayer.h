#pragma once

#include "UglyEngine.h"

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
    
            Ref<Texture2d> m_CheckerboardTexture;
    
            glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
    
            uint32_t m_MapWidth, m_MapHeight;
            std::unordered_map<char, Ref<SubTexture2d>> s_TextureMap;
    };
};
