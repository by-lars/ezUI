#pragma once

#include <string>
typedef struct GLFWwindow GLFWwindow;

namespace ez {
	
	struct UIAppSpecification {
		std::string Title;
		int Width;
		int Height;
	};

	class UIApp {
	public:
		~UIApp();
		static UIApp* Create(const UIAppSpecification& settings);
		void Run();

	private:
		UIApp(const UIAppSpecification& settings);
		
		static void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height);

		GLFWwindow* m_Window;
		UIAppSpecification m_Spec;
	};
}