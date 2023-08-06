#pragma once
#include "ez/Core/Base.hpp"
#include "ez/Core/StrongHandle.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"
#include "ez/Graphics/API/Enums.hpp"
#include "ez/Graphics/Renderer/Color.hpp"

#include <glm/glm.hpp>

namespace ez::gfx {
	EZ_MAKE_STRONG_HANDLE(Brush);
	EZ_MAKE_STRONG_HANDLE(Sprite);

	class Renderer2D {
	public:
        /**
         * Initializes the Renderer2D
         * @param width width of the render target
         * @param height height of the render target
         */
		static void init(uint32_t width, uint32_t height);

        /**
         * Shuts the Renderer2D down and frees resources
         */
		static void shutdown();

        /**
         * Set the current view matrix
         * @param matrix new view matrix
         */
		static void set_view_matrix(const glm::mat4& matrix);

        /**
         * Set the size of the render target
         * @param width new width
         * @param height new height
         */
		static void set_render_size(uint32_t width, uint32_t height);

        /**
         * Starts a new frame
         */
		static void begin_frame();

        /**
         * Ends and draws the current frame
         */
		static void end_frame();

        /**
         * Creates a new single colored brush
         * @param color color of the brush
         * @return new single colored Brush
         */
		static Brush create_solid_color_brush(Color color);

        /**
         * Creates a new gradient brush between two colors
         * @param color1 first Color
         * @param color2 second Color
         * @return new gradient brush
         */
		static Brush create_gradient_brush(Color color1, Color color2);

        /**
         * Draws a rotated rectangle
         * @param brush brush to use
         * @param position position of the top left corner
         * @param size width and height
         * @param rotation rotation around each axis
         */
		static void draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);

        /**
         * Draws a rectangle
         * @param brush brush to use
         * @param position position of the top left corner
         * @param size width and height
         */
		static void draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size);
	};
}