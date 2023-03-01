#include "Core/GFXApp.h"
#include "Core/Base.h"
#include "Graphics/Renderer2D.h"
#include "Debug/Profiling.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ez {
	GFXApp* GFXApp::Create(const GFXApp::Specification& settings) {
		return new GFXApp(settings);
	}

	void GFXApp::OnFramebufferSizeChanged(GLFWwindow* window, int width, int height) {
		if (width > 0 && height > 0) {
			GFXApp* app = (GFXApp*)glfwGetWindowUserPointer(window);
			app->m_Spec.Width = width;
			app->m_Spec.Height = height;
			app->m_Spec.OnResizeCallback(width, height);
		}
	}

	GFXApp::GFXApp(const GFXApp::Specification& settings) {
		ez::Logger::Init();

		EZ_PROFILE_BEGIN_SESSION("Startup");

		EZ_CORE_LOG("ezUI Version v", ez::VERSION_MAJOR, ".", ez::VERSION_MINOR, ".", ez::VERSION_PATCH);

		m_Spec = settings;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef EZ_BUILD_DEBUG_MODE
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif

		m_Window = glfwCreateWindow(m_Spec.Width, m_Spec.Height, m_Spec.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetFramebufferSizeCallback(m_Window, GFXApp::OnFramebufferSizeChanged);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSwapInterval(1);

		EZ_PROFILE_END_SESSION();
	}

	GFXApp::~GFXApp() {
		EZ_PROFILE_BEGIN_SESSION("Shutdown");

		glfwTerminate();

		EZ_PROFILE_END_SESSION();
	}

	int GFXApp::GetWidth() { return m_Spec.Width; }
	int GFXApp::GetHeight() { return m_Spec.Height; }

	void GFXApp::Run() {
		EZ_PROFILE_BEGIN_SESSION("Runtime");

		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();

			static double lastFrameTime = 0;
			double currentFrameTime = glfwGetTime();
			double delta = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;
			
			m_Spec.OnDrawCallback(delta);

			glfwSwapBuffers(m_Window);	
		}

		EZ_PROFILE_END_SESSION();
	}
}