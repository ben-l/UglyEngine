#include <UglyEngine.h>
#include <OpenGLShader.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Ugly::Layer
{
    public:
        ExampleLayer()
			: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
        {
        	m_VertexArray.reset(Ugly::VertexArray::Create());

        	float vertices[3 * 7] = {
        	   -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        	    0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        	    0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f 
        	};

            Ugly::Ref<Ugly::VertexBuffer> vertexBuffer;
        	vertexBuffer.reset(Ugly::VertexBuffer::Create(vertices, sizeof(vertices)));
			Ugly::BufferLayout layout = {
        	    { Ugly::ShaderDataType::Float3, "a_Position" },
        	    { Ugly::ShaderDataType::Float4, "a_Color" }
        	};
        	vertexBuffer->SetLayout(layout);
        	m_VertexArray->AddVertexBuffer(vertexBuffer);

        	uint32_t indices[3] = { 0, 1, 2 };
            Ugly::Ref<Ugly::IndexBuffer> indexBuffer;
        	indexBuffer.reset(Ugly::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        	m_VertexArray->SetIndexBuffer(indexBuffer);

        	m_SquareVA.reset(Ugly::VertexArray::Create());

        	float squareVertices[3 * 4] = {
        	   -0.5f,-0.5f, 0.0f,
        	    0.5f,-0.5f, 0.0f, 
        	    0.5f, 0.5f, 0.0f,
        	   -0.5f, 0.5f, 0.0f
        	};

            Ugly::Ref<Ugly::VertexBuffer> squareVB; 
        	squareVB.reset(Ugly::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
			squareVB->SetLayout({
        	    { Ugly::ShaderDataType::Float3, "a_Position" }
			});
        	m_SquareVA->AddVertexBuffer(squareVB);

        	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
            Ugly::Ref<Ugly::IndexBuffer> squareIB;
        	squareIB.reset(Ugly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

        	m_Shader.reset(Ugly::Shader::Create(vertexSrc, fragmentSrc));

        	std::string vertexSrc2 = R"(
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

        	flatColorShader.reset(Ugly::Shader::Create(vertexSrc2, flatColorShaderFragmentSrc));
        }

        void OnUpdate(Ugly::Timestep ts) override
        {
			if (Ugly::Input::IsKeyPressed(UE_KEY_LEFT))
				m_CameraPosition.x -= m_CameraMoveSpeed * ts;
			else if (Ugly::Input::IsKeyPressed(UE_KEY_RIGHT))
				m_CameraPosition.x += m_CameraMoveSpeed * ts;

			if (Ugly::Input::IsKeyPressed(UE_KEY_UP))
				m_CameraPosition.y += m_CameraMoveSpeed * ts;
			else if (Ugly::Input::IsKeyPressed(UE_KEY_DOWN))
				m_CameraPosition.y -= m_CameraMoveSpeed * ts;

			if (Ugly::Input::IsKeyPressed(UE_KEY_H))
				m_SquarePosition.x -= m_SquareMoveSpeed * ts;
			else if (Ugly::Input::IsKeyPressed(UE_KEY_L))
				m_SquarePosition.x += m_SquareMoveSpeed * ts;

			if (Ugly::Input::IsKeyPressed(UE_KEY_K))
				m_SquarePosition.y += m_SquareMoveSpeed * ts;
			else if (Ugly::Input::IsKeyPressed(UE_KEY_J))
				m_SquarePosition.y -= m_SquareMoveSpeed * ts;

			if (Ugly::Input::IsKeyPressed(UE_KEY_A))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			if (Ugly::Input::IsKeyPressed(UE_KEY_D))
				m_CameraRotation -= m_CameraRotationSpeed * ts;


			Ugly::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Ugly::RenderCommand::Clear();

            m_Camera.SetPosition(m_CameraPosition);
            m_Camera.SetRotation(m_CameraRotation);

            Ugly::Renderer::BeginScene(m_Camera);

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

			//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);
            //Ugly::Renderer::Submit(flatColorShader, m_SquareVA, transform);
			Ugly::Renderer::Submit(m_Shader, m_VertexArray);
            Ugly::Renderer::EndScene();

        }

        virtual void OnImGuiRender() override
		{
            ImGui::Begin("Settings");
            ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
            ImGui::End();
        }

        void OnEvent(Ugly::Event& event) override
        {
            /*
            if(event.GetEventType() == Ugly::EventType::KeyPressed)
            {
                Ugly::KeyPressedEvent& e = (Ugly::KeyPressedEvent&)event;
                UE_TRACE("{0}", (char)e.GetKeyCode());
            }
            */
        }
		
	private:
        Ugly::Ref<Ugly::Shader> m_Shader;
        Ugly::Ref<Ugly::VertexArray> m_VertexArray;

        Ugly::Ref<Ugly::Shader> flatColorShader;
        Ugly::Ref<Ugly::VertexArray> m_SquareVA;

		Ugly::OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 5.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;

		glm::vec3 m_SquarePosition;
		float m_SquareMoveSpeed = 3.0f;

        glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Ugly::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }
    ~Sandbox()
    {
    }
};

Ugly::Application* Ugly::CreateApplication()
{
    return new Sandbox();
}
