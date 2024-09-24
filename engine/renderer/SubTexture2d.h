#pragma once
#include "Texture.h"

#include <glm/glm.hpp>

namespace Ugly {
    class SubTexture2d {
        public:
            SubTexture2d(const Ref<Texture2d>& texture, const glm::vec2& min, const glm::vec2& max);

            const Ref<Texture2d> GetTexture() const { return m_Texture; }
            const glm::vec2* GetTexCoords() const { return m_TexCoords; }

            static Ref<SubTexture2d> CreateFromCoords(const Ref<Texture2d>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1, 1});
        private:
            Ref<Texture2d> m_Texture;
            glm::vec2 m_TexCoords[4];
    };
}
