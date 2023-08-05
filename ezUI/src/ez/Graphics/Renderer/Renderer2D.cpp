#include "Renderer2D.hpp"
#include "ez/Core/Base.hpp"
#include "ez/Core/Profiling.hpp"
#include "ez/Core/StrongHandle.hpp"

#include "ez/Graphics/API/RenderAPI.hpp"
#include "ez/Graphics/API/OpenGL/GL_BuiltinShader.hpp"
#include "ez/Graphics/Renderer/Font.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ez::gfx {
	constexpr int MAX_QUADS = 20000;

	struct QuadData {
		float data[8];
		uint32_t data_2[4];
	};

	static Ref<RenderAPI> s_device;

	struct RendererData {
		Ref<TextureArray> brushes;
		Ref<StreamStorage> quad_storage;
		Ref<Shader> quad_shader;

		Ref<Font> font;

		uint32_t render_width;
		uint32_t render_height;
	};

	static RendererData s_Data;

	void Renderer2D::init(uint32_t width, uint32_t height) {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_ALLOC("Initializing Renderer2D");
        s_device = RenderAPI::create(ez::gfx::API::OPENGL);

		//Load Shader
		s_Data.quad_shader = s_device->create_shader({
                                                             {Shader::Type::VERTEX,   ez::gfx::GL_QUAD_VERTEX_SHADER},
                                                             {Shader::Type::FRAGMENT, ez::gfx::GL_QUAD_FRAGMENT_SHADER}
                                                     });
        s_Data.quad_shader->bind();

		s_Data.quad_storage = s_device->create_stream_storage(MAX_QUADS, sizeof(QuadData));


		s_Data.brushes = s_device->create_texture_array(2, 2, Format::RGBA, Filter::LINEAR);
        s_Data.brushes->bind_to_slot(1);
        s_Data.quad_shader->set("uBrushTex", 1);
		EZ_CORE_DEBUG("Max Texture Array Layers: ", s_Data.brushes->get_max_layers());

		//s_Data.Font = CreateRef<Font>("C:\\Windows\\Fonts\\segoeui.ttf", s_device);
		//s_Data.Font->get_texture()->bind_to_slot(2);
		
		//Setup default state
        s_device->set_clear_color(0.15f, 0.15f, 0.15f, 1.0f);
        set_render_size(width, height);
	}

	void Renderer2D::shutdown() {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_DEALLOC("Shutting down Renderer2D");
	}

	void Renderer2D::set_render_size(uint32_t width, uint32_t height) {
		EZ_PROFILE_FUNCTION();

		s_Data.render_width = width;
		s_Data.render_height = height;

		float fovy = glm::radians(45.0f);
		float aspect = (float)s_Data.render_width / (float)s_Data.render_height;
		float depthZ = (-1.0f * s_Data.render_height) / (2.0f * tanf(fovy / 2.0f));
		glm::mat4 proj = glm::perspective(fovy, aspect, 1.0f, s_Data.render_height * 2.0f);

        s_Data.quad_shader->set("uProj", proj);
        s_Data.quad_shader->set("uOffset", depthZ);

        s_device->set_viewport(0, 0, width, height);
	}

	void Renderer2D::set_view_matrix(const glm::mat4& matrix) {
		EZ_PROFILE_FUNCTION();
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(-0.5f * s_Data.render_width, 0.5f * s_Data.render_height, 0.0f));
		view = glm::scale(view, glm::vec3(1.0f, -1.0f, 1.0f));
        s_Data.quad_shader->set("uView", view);
	}

	void Renderer2D::begin_frame() {
		EZ_PROFILE_FUNCTION();
        s_Data.quad_storage->begin_frame();
	}

	void Renderer2D::end_frame() {
		EZ_PROFILE_FUNCTION();
        s_device->clear();
        s_device->draw_arrays(DrawMode::TRIANGLES, s_Data.quad_storage->count() * 6, s_Data.quad_storage->offset() * 6);
        s_Data.quad_storage->end_frame();
	}

	Brush Renderer2D::create_solid_color_brush(Color color) {
		uint8_t colorData[]{
                color.r, color.g, color.b, color.a,
                color.r, color.g, color.b, color.a,
                color.r, color.g, color.b, color.a,
                color.r, color.g, color.b, color.a,
		};

		return Brush((int32_t) s_Data.brushes->push_back(colorData));
	}

	Brush Renderer2D::create_gradient_brush(Color color1, Color color2) {
		uint8_t colorData[]{
                color1.r, color1.g, color1.b, color1.a,
                color1.r, color1.g, color1.b, color1.a,
                color2.r, color2.g, color2.b, color2.a,
                color2.r, color2.g, color2.b, color2.a,
		};

		return Brush((int32_t) s_Data.brushes->push_back(colorData));
	}


	void Renderer2D::draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation) {
		EZ_PROFILE_FUNCTION();

		QuadData data{
			{position.x, position.y, position.z, size.x, size.y, rotation.x, rotation.y, rotation.z}, {(uint32_t)brush.get(), 0, 0, 0}
		};

        s_Data.quad_storage->push_back(&data);
	}

	void Renderer2D::draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size) {
		EZ_PROFILE_FUNCTION();

		QuadData data {
			{position.x, position.y, position.z, size.x, size.y, 0, 0, 0}, {(uint32_t)brush.get(), 0, 0, 0}
		};

        s_Data.quad_storage->push_back(&data);
	}
}