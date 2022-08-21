#include <iostream>
#include "Core/UIApp.h"
#include "Core/Base.h"

int main() {
    ez::UIAppSpecification settings;
    settings.Title = "My Application";
    settings.Width = 800;
    settings.Height = 600;
    
    ez::UIApp* app = ez::UIApp::Create(settings);
    app->Run();

    EZ_LOG("App Exiting...");

    delete app;
}
