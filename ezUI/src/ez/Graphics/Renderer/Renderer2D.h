#pragma once
#include "ez/Core/StrongHandle.h"
#include <glm/glm.hpp>

namespace ez::gfx {
	EZ_MAKE_STRONG_HANDLE(Brush);
	EZ_MAKE_STRONG_HANDLE(Sprite);

	class Renderer2D {
	public:
		Renderer2D(uint32_t width, uint32_t height);
		~Renderer2D();

		void SetViewMatrix(const glm::mat4& matrix);
		void SetRenderSize(uint32_t width, uint32_t height);

		void BeginScene();
		void EndScene();

		Brush CreateSolidColorBrush(const glm::vec4& color);

		void DrawRect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);
		void DrawRect(Brush brush, const glm::vec3& position, const glm::vec2& size);

	private:

	};
}