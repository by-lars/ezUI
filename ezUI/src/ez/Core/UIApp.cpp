#include "ez/Core/UIApp.hpp"
#include "ez/Core/Base.hpp"
#include "ez/Core/Profiling.hpp"
#include "ez/Graphics/Renderer/Renderer2D.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ez {
    UIApp* ez::UIApp::s_instance = nullptr;

	gfx::Brush defaultBrush;
	gfx::Brush defaultBrush2;
	gfx::Brush gradientBrush;

	void UIApp::on_framebuffer_size_changed(GLFWwindow* window, int width, int height) {
		if (width > 0 && height > 0) {
            gfx::Renderer2D::set_render_size(width, height);
            UIApp::get().m_spec.width = width;
            UIApp::get().m_spec.height = height;
		}
	}

	UIApp::UIApp(const UIApp::Specification& settings) {
        s_instance = this;

		ez::Logger::init();
		
		EZ_PROFILE_BEGIN_SESSION("Startup");

		EZ_CORE_LOG("ezUI Version v", ez::VERSION_MAJOR, ".", ez::VERSION_MINOR, ".", ez::VERSION_PATCH);

        m_spec = settings;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef EZ_BUILD_DEBUG_MODE
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif

        m_window = glfwCreateWindow(m_spec.width, m_spec.height, m_spec.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, UIApp::on_framebuffer_size_changed);
		glfwSetWindowUserPointer(m_window, this);

        gfx::Renderer2D::init(m_spec.width, m_spec.height);

		defaultBrush = gfx::Renderer2D::create_solid_color_brush(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		defaultBrush2 = gfx::Renderer2D::create_solid_color_brush(glm::vec4(0.0, 1.0, 1.0, 1.0));

		gradientBrush = gfx::Renderer2D::create_gradient_brush(glm::vec4(0.98f, 0.18f, 0.63f, 1.0f),
                                                               glm::vec4(0.94f, 0.24f, 0.3f, 1.0f));
			 
		EZ_PROFILE_END_SESSION();
	}

	UIApp::~UIApp() {
		EZ_PROFILE_BEGIN_SESSION("shutdown");

        gfx::Renderer2D::shutdown();
		glfwTerminate();

		EZ_PROFILE_END_SESSION();
	}

	void UIApp::run() {
		EZ_PROFILE_BEGIN_SESSION("Runtime");

		while (!glfwWindowShouldClose(m_window)) {
			//glfwWaitEvents();
			glfwPollEvents();

			static double lastFrameTime = 0;
			double currentFrameTime = glfwGetTime();
			double delta = currentFrameTime - lastFrameTime;
			std::string frameRate = "FPS: ";
			frameRate += std::to_string((1 / delta));
			glfwSetWindowTitle(m_window, frameRate.c_str());

            gfx::Renderer2D::begin_frame();
            gfx::Renderer2D::set_view_matrix(glm::mat4(1.0f));

            gfx::Renderer2D::draw_rect(gradientBrush, glm::vec3(0, 0, 0), glm::vec3(m_spec.width, m_spec.height, 0),
                                       glm::vec3(0, 0, 0));

			for (int x = 0; x < 16; x++) {
				for (int y = 0; y < 16; y++) {
                    gfx::Renderer2D::draw_rect(defaultBrush,
                                               glm::vec3(x * 65, y * 65, sin(4 * currentFrameTime + x + y) * 10),
                                               glm::vec2(64, 64), glm::vec3(0));
				}
			}

            gfx::Renderer2D::end_frame();
		

			glfwSwapBuffers(m_window);
			lastFrameTime = currentFrameTime;
		}

		EZ_PROFILE_END_SESSION();
	}

    UIApp& UIApp::get() {
        return *s_instance;
    }
}