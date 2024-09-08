#include <Sandbox2d.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGLShader.h>

Sandbox2d::Sandbox2d() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2d::OnAttach(){

    m_SquareVA = Ugly::VertexArray::Create();

    float squareVertices[5 * 4] = {
       -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
       -0.5f, 0.5f, 0.0f
    };

    Ugly::Ref<Ugly::VertexBuffer> squareVB; 
    squareVB.reset(Ugly::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
        { Ugly::ShaderDataType::Float3, "a_Position" },
	});
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Ugly::Ref<Ugly::IndexBuffer> squareIB;
    squareIB.reset(Ugly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB);
	

    m_FlatColorShader = Ugly::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2d::OnDetach(){
}

void Sandbox2d::OnUpdate(Ugly::Timestep ts){
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
	Ugly::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Ugly::RenderCommand::Clear();

    Ugly::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Ugly::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Ugly::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    Ugly::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Ugly::Renderer::EndScene();
}

void Sandbox2d::OnImGuiRender(){
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2d::OnEvent(Ugly::Event& e){
    m_CameraController.OnEvent(e);
}
