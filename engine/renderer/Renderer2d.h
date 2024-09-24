#pragma once
#include "OrthographicCamera.h"

#include "Texture.h"
#include "SubTexture2d.h"

namespace Ugly {
    class Renderer2d {
        public:
            static void Init();
            static void Shutdown();

            static void BeginScene(const OrthographicCamera& camera);
            static void EndScene();
            static void Flush();

            static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
            static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
            static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2d>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
            static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2d>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
            static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2d>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
            static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2d>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


            // Rotation is in radians
            static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2d>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2d>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2d>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2d>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
            struct Statistics {
                uint32_t DrawCalls = 0;
                uint32_t QuadCount = 0;
                uint32_t GetTotalVertexCount() { return QuadCount * 4; }
                uint32_t GetTotalIndexCount() { return QuadCount * 6; }
            };

            static void ResetStats();
            static Statistics GetStats();
        private:
            static void FlushAndReset();
    };
}
