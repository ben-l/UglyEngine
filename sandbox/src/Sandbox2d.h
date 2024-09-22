#pragma once

#include "UglyEngine.h"
#include "ParticleSystem.h"

class Sandbox2d : public Ugly::Layer {
    public:
        Sandbox2d();
        virtual ~Sandbox2d() = default;
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        void OnUpdate(Ugly::Timestep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Ugly::Event& e) override;
    private:
		Ugly::OrthographicCameraController m_CameraController;

        // Temp
        Ugly::Ref<Ugly::VertexArray> m_SquareVA;
        Ugly::Ref<Ugly::Shader> m_FlatColorShader;

        Ugly::Ref<Ugly::Texture2d> m_CheckerboardTexture;

        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

        ParticleSystem m_ParticleSystem;
        ParticleProps m_Particle;
};
