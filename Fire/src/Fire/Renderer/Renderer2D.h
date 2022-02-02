#pragma once

#include "Fire/Renderer/OrthographicCamera.h"
#include "Fire/Renderer/Texture.h"
#include "Fire/Renderer/SubTexture2D.h"

namespace Fire {

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        // Primitives - Quad
        // Color, no texture, vec2 and vec3
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation=0.0f);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation=0.0f);
        // Texture, tint colour, vec2 and vec3
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation=0.0f, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation=0.0f, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        // Sub-texture (from sprite-sheets), vec2 and vec3
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float rotation=0.0f, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float rotation=0.0f, float tilingFactor=1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        // Stats
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() { return QuadCount * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();
    private:
        static void FlushAndReset();
        static void _DrawQuad(const glm::vec3& position, const glm::vec2& size, float textureIndex, const glm::vec2* textureCoords, float rotation, float tilingFactor, const glm::vec4& tintColor);
    };

}
