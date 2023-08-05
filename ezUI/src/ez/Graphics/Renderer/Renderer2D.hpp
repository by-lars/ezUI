#pragma once
#include "ez/Core/Base.hpp"
#include "ez/Core/StrongHandle.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"
#include "ez/Graphics/API/DeviceResources.hpp"
#include "ez/Graphics/Renderer/Color.hpp"

#include <glm/glm.hpp>

namespace ez::gfx {
	EZ_MAKE_STRONG_HANDLE(Brush);
	EZ_MAKE_STRONG_HANDLE(Sprite);

	class Renderer2D {
	public:
		static void init(uint32_t width, uint32_t height);
		static void shutdown();

		static void set_view_matrix(const glm::mat4& matrix);
		static void set_render_size(uint32_t width, uint32_t height);

		static void begin_frame();
		static void end_frame();

		static Brush create_solid_color_brush(Color color);
		static Brush create_gradient_brush(Color color1, Color color2);

		static void draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);
		static void draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size);
	};
}