#include <iostream>

#include "ez/ez.h"

int main() {
    ez::UIApp::Specification settings;
    settings.Title = "My Application";
    settings.Width = 800;
    settings.Height = 600;
    
    ez::UIApp* app = ez::UIApp::Create(settings);
    app->Run();
   
    EZ_LOG("App Exiting...");

    delete app;
}
