#pragma once
#include "ez/Core/Base.hpp"
#include <string>
typedef struct GLFWwindow GLFWwindow;

namespace ez {
	class UIApp {
	public:
		struct Specification {
			std::string title;
			int width;
			int height;
		};

        explicit UIApp(const UIApp::Specification& settings);
		~UIApp();

		static UIApp& get();

		void run();
    private:
		static void on_framebuffer_size_changed(GLFWwindow* window, int width, int height);

		GLFWwindow* m_window;

        static UIApp* s_instance;

        Specification m_spec;
	};
}