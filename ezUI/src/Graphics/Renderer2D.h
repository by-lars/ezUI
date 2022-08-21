#pragma once
#include "Utility/StrongHandle.h"
#include <glm/glm.hpp>


namespace ez {
	class Renderer2D {
	public:
		static void Init(uint32_t width, uint32_t height);
		static void Shutdown();

		static void SetViewMatrix(const glm::mat4& matrix);
		static void SetRenderSize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		//static BrushHandle CreateSolidColorBrush(glm::vec4 color);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);
	};
}