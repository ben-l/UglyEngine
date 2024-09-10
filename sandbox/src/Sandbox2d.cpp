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
    m_CheckerboardTexture = Ugly::Texture2d::Create("assets/textures/Checkerboard.jpg");
}

void Sandbox2d::OnDetach(){
    UE_PROFILE_FUNCTION();
}

void Sandbox2d::OnUpdate(Ugly::Timestep ts){

    UE_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    {
        UE_PROFILE_SCOPE("Renderer_Prep");
	    Ugly::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	    Ugly::RenderCommand::Clear();
    }

    {
        UE_PROFILE_SCOPE("Renderer_Draw");
        Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());
        //Ugly::Renderer2d::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Ugly::Renderer2d::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
        Ugly::Renderer2d::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        //Ugly::Renderer2d::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
        Ugly::Renderer2d::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, -65.0f, m_CheckerboardTexture, 10.0f, {0.9f, 0.3f, 0.6f, 0.8f});
        Ugly::Renderer2d::EndScene();
    }

}

void Sandbox2d::OnImGuiRender(){
    UE_PROFILE_FUNCTION();
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2d::OnEvent(Ugly::Event& e){
    m_CameraController.OnEvent(e);
}
