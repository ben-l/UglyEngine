#include <Sandbox2d.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2d::Sandbox2d() : Layer("Sandbox2d"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2d::OnAttach()
{
    UE_PROFILE_FUNCTION();
    m_CheckerboardTexture = Ugly::Texture2d::Create("assets/textures/Checkerboard.png");

    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2d::OnDetach(){
    UE_PROFILE_FUNCTION();
}

void Sandbox2d::OnUpdate(Ugly::Timestep ts){

    UE_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Ugly::Renderer2d::ResetStats();
    {
        UE_PROFILE_SCOPE("Renderer_Prep");
	    Ugly::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.1f, 1 });
	    Ugly::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        UE_PROFILE_SCOPE("Renderer_Draw");
        Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());
		Ugly::Renderer2d::DrawRotatedQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(-45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
		Ugly::Renderer2d::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Ugly::Renderer2d::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		//Ugly::Renderer2d::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		Ugly::Renderer2d::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 1.0f);
		Ugly::Renderer2d::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 1.0f);
        Ugly::Renderer2d::EndScene();

        Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f){
                glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.9f };
                Ugly::Renderer2d::DrawQuad({x, y}, { 0.45f, 0.45f }, color);
            }
        }
        Ugly::Renderer2d::EndScene();
    }


    if (Ugly::Input::IsMouseButtonPressed(UE_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Ugly::Input::GetMousePosition();
		auto width = Ugly::Application::Get().GetWindow().GetWidth();
		auto height = Ugly::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
        // number of particles
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

}

void Sandbox2d::OnImGuiRender()
{
    UE_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Ugly::Renderer2d::GetStats();
    ImGui::Text("Renderer2d Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2d::OnEvent(Ugly::Event& e){
    m_CameraController.OnEvent(e);
}
