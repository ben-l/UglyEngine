#include <Sandbox2d.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGLShader.h>

Sandbox2d::Sandbox2d() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2d::OnAttach(){

}

void Sandbox2d::OnDetach(){
}

void Sandbox2d::OnUpdate(Ugly::Timestep ts){
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
	Ugly::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Ugly::RenderCommand::Clear();

    Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());

    Ugly::Renderer2d::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});

    Ugly::Renderer2d::EndScene();

    //TODO Add these functions - Shader::SetMat4, Shader::SetFloat4
    //std::dynamic_pointer_cast<Ugly::OpenGLShader>(m_FlatColorShader)->Bind();
    //std::dynamic_pointer_cast<Ugly::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2d::OnImGuiRender(){
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2d::OnEvent(Ugly::Event& e){
    m_CameraController.OnEvent(e);
}
