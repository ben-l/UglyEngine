#include <Sandbox2d.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2d::Sandbox2d() : Layer("Sandbox2d"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2d::OnAttach()
{
    UE_PROFILE_FUNCTION();
    m_CheckerboardTexture = Ugly::Texture2d::Create("assets/textures/Checkerboard.png");
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
		Ugly::Renderer2d::DrawRotatedQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, -45.0f, {0.8f, 0.2f, 0.3f, 1.0f});
		Ugly::Renderer2d::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Ugly::Renderer2d::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 1.0f, 0.3f, 0.2f, 1.0f });
		//Ugly::Renderer2d::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		Ugly::Renderer2d::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 1.0f);
		Ugly::Renderer2d::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 1.0f);
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
