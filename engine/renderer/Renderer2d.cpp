#include "uepch.h"
#include "Renderer2d.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ugly {

    struct Renderer2dStorage {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
        Ref<Shader> TextureShader;
    };

    static Renderer2dStorage* s_Data;

    void Renderer2d::Init() {

        s_Data = new Renderer2dStorage();

        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[5 * 4] = {
           -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
            0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
           -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<VertexBuffer> squareVB; 
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	    squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
	    });
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIB);
	    

        s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);
    }

    void Renderer2d::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2d::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2d::EndScene()
    {
    }

    void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetFloat4("u_Color", color);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f });
        s_Data->FlatColorShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2d::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2d>& texture)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture);
    }

    void Renderer2d::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2d>& texture)
    {
        s_Data->TextureShader->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f });
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        texture->Bind();

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}