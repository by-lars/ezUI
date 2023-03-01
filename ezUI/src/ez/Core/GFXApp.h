#pragma once

#include <string>
#include <functional>
typedef struct GLFWwindow GLFWwindow;

namespace ez {
	class GFXApp {
	public:
		struct Specification {
			std::string Title;
			int Width;
			int Height;
			std::function<void(double delta)> OnDrawCallback;
			std::function<void(int, int)> OnResizeCallback;
		};

		~GFXApp();
		static GFXApp* Create(const GFXApp::Specification& settings);
		void Run();

		int GetWidth();
		int GetHeight();

	private:
		GFXApp(const GFXApp::Specification& settings);

		static void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height);

		GLFWwindow* m_Window;
		Specification m_Spec;
	};
}
