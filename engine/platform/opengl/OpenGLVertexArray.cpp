#include <uepch.h>
#include <OpenGLVertexArray.h>

#include <glad/glad.h>

namespace Ugly {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type){
        switch (type){
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT; 
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_BOOL;
        }
        UE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
            
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        UE_PROFILE_FUNCTION();
        glCreateVertexArrays(1, &m_RendererID);
    }
     
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        UE_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const {
        UE_PROFILE_FUNCTION();
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const {
        UE_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        UE_PROFILE_FUNCTION();

        UE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout ){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 
                    element.GetComponentCount(), 
                    ShaderDataTypeToOpenGLBaseType(element.Type), 
                    element.Normalized ? GL_TRUE : GL_FALSE, 
                    layout.GetStride(), 
                    (const void*)element.Offset);
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        UE_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
