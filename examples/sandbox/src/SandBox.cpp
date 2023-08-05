#include <iostream>

#include "ez/ez.hpp"

int main() {
    ez::UIApp::Specification spec {
        .title = "My Application",
        .width = 800,
        .height = 600
    };

    auto app = new ez::UIApp(spec);
    app->run();
   
    EZ_LOG("App Exiting...");
}
