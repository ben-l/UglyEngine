#pragma once
#include "FrameBuffer.h"

namespace Ugly {
    class OpenGLFrameBuffer : public FrameBuffer {
        public:
            OpenGLFrameBuffer(const FrameBufferSpecification& spec);
            virtual ~OpenGLFrameBuffer();
            void Invalidate();

            void Bind() override;
            void Unbind() override;

            virtual void Resize(uint32_t width, uint32_t height) override;
            virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

            virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

            virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { UE_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }

            virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
        private:
            uint32_t m_RendererID = 0;
            FrameBufferSpecification m_Specification;

            std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
            FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;

            std::vector<uint32_t> m_ColorAttachments;
            uint32_t m_DepthAttachment = 0;
    };
};
