#include <UglyEngine.h>
#include <EntryPoint.h>

#include <OpenGLShader.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Sandbox2d.h>


class ExampleLayer : public Ugly::Layer
{
    public:
        ExampleLayer()
			: Layer("Example"), m_CameraController(1280.0f / 720.0f)
        {
        	m_VertexArray = Ugly::VertexArray::Create();

        	float vertices[3 * 7] = {
        	   -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        	    0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        	    0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f 
        	};

            Ugly::Ref<Ugly::VertexBuffer> vertexBuffer = Ugly::VertexBuffer::Create(vertices, sizeof(vertices));

			Ugly::BufferLayout layout = {
        	    { Ugly::ShaderDataType::Float3, "a_Position" },
        	    { Ugly::ShaderDataType::Float4, "a_Color" }
        	};
        	vertexBuffer->SetLayout(layout);
        	m_VertexArray->AddVertexBuffer(vertexBuffer);

        	uint32_t indices[3] = { 0, 1, 2 };
            Ugly::Ref<Ugly::IndexBuffer> indexBuffer = Ugly::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        	m_VertexArray->SetIndexBuffer(indexBuffer);

        	m_SquareVA = Ugly::VertexArray::Create();

        	float squareVertices[5 * 4] = {
        	   -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
        	    0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
        	    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        	   -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        	};

            Ugly::Ref<Ugly::VertexBuffer> squareVB = Ugly::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
			squareVB->SetLayout({
        	    { Ugly::ShaderDataType::Float3, "a_Position" },
        	    { Ugly::ShaderDataType::Float2, "a_TexCoord" }
			});
        	m_SquareVA->AddVertexBuffer(squareVB);

        	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
            Ugly::Ref<Ugly::IndexBuffer> squareIB = Ugly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        	m_SquareVA->SetIndexBuffer(squareIB);
			

        	std::string vertexSrc = R"(
        	    #version 330 core

        	    layout(location = 0) in vec3 a_Position;
        	    layout(location = 1) in vec4 a_Color;

        	    uniform mat4 u_ViewProjection;
        	    uniform mat4 u_Transform;

        	    out vec3 v_Position;
        	    out vec4 v_Color;

        	    void main()
				{
        	        v_Position = a_Position;
        	        v_Color = a_Color;
        	        gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        	    }
        	)";

        	std::string fragmentSrc = R"(
        	    #version 330 core

        	    layout(location = 0) out vec4 color;

        	    in vec3 v_Position;
        	    in vec4 v_Color;

        	    void main()
				{
        	        color = vec4(v_Position * 0.5 + 0.5, 1.0);
        	        color = v_Color;
        	    }
        	)";

        	m_Shader = Ugly::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);


        	std::string flatColorShaderVertexSrc = R"(
        	    #version 330 core

        	    layout(location = 0) in vec3 a_Position;

        	    uniform mat4 u_ViewProjection;
        	    uniform mat4 u_Transform;

        	    out vec3 v_Position;

        	    void main()
				{
        	        v_Position = a_Position;
        	        gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        	    }
        	)";

        	std::string flatColorShaderFragmentSrc = R"(
        	    #version 330 core

        	    layout(location = 0) out vec4 color;
                
				in vec3 v_Position;

                uniform vec3 u_Color;

        	    void main()
				{
        	        color = vec4(u_Color, 1.0);
        	    }
        	)";

        	std::string textureShaderVertexSrc = R"(
        	    #version 330 core

        	    layout(location = 0) in vec3 a_Position;
        	    layout(location = 1) in vec2 a_TexCoord;

        	    uniform mat4 u_ViewProjection;
        	    uniform mat4 u_Transform;

                out vec2 v_TexCoord;


        	    void main()
				{
                    v_TexCoord = a_TexCoord;
        	        gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        	    }
        	)";

        	std::string textureShaderFragmentSrc = R"(
        	    #version 330 core

        	    layout(location = 0) out vec4 color;
                
				in vec2 v_TexCoord;

                uniform sampler2D u_Texture;

        	    void main()
				{
        	        color = texture(u_Texture, v_TexCoord);
        	    }
        	)";

        	flatColorShader = Ugly::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

        	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

        	m_Texture = Ugly::Texture2d::Create("assets/textures/Checkerboard.jpg");

            std::dynamic_pointer_cast<Ugly::OpenGLShader>(textureShader)->Bind();
            std::dynamic_pointer_cast<Ugly::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
        }

        void OnUpdate(Ugly::Timestep ts) override
        {

            // Update
            m_CameraController.OnUpdate(ts);

            // Render
			Ugly::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Ugly::RenderCommand::Clear();

            Ugly::Renderer::BeginScene(m_CameraController.GetCamera());


			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

            std::dynamic_pointer_cast<Ugly::OpenGLShader>(flatColorShader)->Bind();
            std::dynamic_pointer_cast<Ugly::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

			for(int y=0; y < 20; y++){
				for(int x=0; x < 20; x++)
				{
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            		Ugly::Renderer::Submit(flatColorShader, m_SquareVA, transform);
				}
			}

            auto textureShader = m_ShaderLibrary.Get("Texture");

            m_Texture->Bind();
            Ugly::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

            // Triangle
			// Ugly::Renderer::Submit(m_Shader, m_VertexArray);
            Ugly::Renderer::EndScene();

        }

        virtual void OnImGuiRender() override
		{
            ImGui::Begin("Settings");
            ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
            ImGui::End();
        }

        void OnEvent(Ugly::Event& e) override
        {
            m_CameraController.OnEvent(e);
        }
		
	private:
        Ugly::ShaderLibrary m_ShaderLibrary;
        Ugly::Ref<Ugly::Shader> m_Shader;
        Ugly::Ref<Ugly::VertexArray> m_VertexArray;

        Ugly::Ref<Ugly::Shader> flatColorShader;
        Ugly::Ref<Ugly::VertexArray> m_SquareVA;

        Ugly::Ref<Ugly::Texture2d> m_Texture;

		Ugly::OrthographicCameraController m_CameraController;
        glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Ugly::Application
{
public:
    Sandbox()
    {
        //PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2d());
    }
    ~Sandbox()
    {
    }
};

Ugly::Application* Ugly::CreateApplication()
{
    return new Sandbox();
}
