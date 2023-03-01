#pragma once
#include "ez/Core/Base.h"
#include "ez/Core/StrongHandle.h"
#include "ez/Graphics/API/RenderAPI.h"
#include "ez/Graphics/API/DeviceResources.h"

#include <glm/glm.hpp>

namespace ez::gfx {
	EZ_MAKE_STRONG_HANDLE(Brush);
	EZ_MAKE_STRONG_HANDLE(Sprite);

	class Renderer2D {
	public:
		static void Init(uint32_t width, uint32_t height);
		static void Shutdown();

		static void SetViewMatrix(const glm::mat4& matrix);
		static void SetRenderSize(uint32_t width, uint32_t height);

		static void BeginFrame();
		static void EndFrame();

		static Brush CreateSolidColorBrush(const glm::vec4& color);

		static void DrawRect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);
		static void DrawRect(Brush brush, const glm::vec3& position, const glm::vec2& size);
	};
}