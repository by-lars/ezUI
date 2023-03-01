#pragma once
#include "ez/Core/Base.h"
#include <string>
typedef struct GLFWwindow GLFWwindow;

namespace ez {
	class UIApp {
	public:
		struct Specification {
			std::string Title;
			int Width;
			int Height;
		};

		~UIApp();
		static UIApp* Create(const UIApp::Specification& settings);
		void Run();

	private:
		UIApp(const UIApp::Specification& settings);
		
		static void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height);

		GLFWwindow* m_Window;
		Specification m_Spec;
	};
}