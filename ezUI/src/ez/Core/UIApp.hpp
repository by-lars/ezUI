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

        /**
         * Creates a new UIApp with the provided specification
         * @param spec Specification
         */
        explicit UIApp(const UIApp::Specification& spec);

        /**
         * Destructor
         */
		~UIApp();

        /**
         * Gets the instance of the UIApp
         * @return current instance of the App
         */
		static UIApp& get();

        /**
         * Runs the app
         */
		void run();
    private:
        /**
         * The framebuffer size changed callback get called once the window has been resized
         * @param window pointer to the window that has been resized
         * @param width new window width
         * @param height new window height
         */
		static void on_framebuffer_size_changed(GLFWwindow* window, int width, int height);

        static UIApp* s_instance;
        GLFWwindow* m_window;
        Specification m_spec;
	};
}