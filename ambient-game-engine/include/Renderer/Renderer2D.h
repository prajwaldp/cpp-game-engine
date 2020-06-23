#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/SubTexture2D.h"

namespace Ambient
{
    class Renderer2D
    {
    public:
        static void Init();

        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);

        static void EndScene();

        static void Flush();

        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& sub_texture,
                float repeat_count = 1.0f);

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& sub_texture,
                float repeat_count = 1.0f);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle,
                const glm::vec4& color);

        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle,
                const glm::vec4& color);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle,
                const Ref<Texture2D>& texture);

        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle,
                const Ref<Texture2D>& texture);

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount()
            {
                return QuadCount * 4;
            }

            uint32_t GetTotalIndexCount()
            {
                return QuadCount * 6;
            }
        };

        Statistics stats;

        static Statistics GetStats();

        static void ResetStats();


    private:
        static void FlushAndReset();
    };
}