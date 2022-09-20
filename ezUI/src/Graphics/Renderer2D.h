#pragma once
#include "Core/StrongHandle.h"
#include "Graphics/DeviceResources.h"
#include <glm/glm.hpp>

namespace ez::gfx {
	EZ_MAKE_STRONG_HANDLE(HBrush);
	EZ_MAKE_STRONG_HANDLE(HSprite);

	class Renderer2D {
	public:
		static void Init(uint32_t width, uint32_t height);
		static void Shutdown();

		static void SetViewMatrix(const glm::mat4& matrix);
		static void SetRenderSize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		static HBrush CreateSolidColorBrush(const glm::vec4& color);

		static void DrawRect(HBrush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);
		static void DrawRect(HBrush brush, const glm::vec3& position, const glm::vec2& size);
	};
}