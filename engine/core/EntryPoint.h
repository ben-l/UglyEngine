#pragma once
#include "Core.h"

#ifdef __linux__

extern Ugly::Application* Ugly::CreateApplication();

int main(int argc, char** argv){
    Ugly::Log::Init();

    UE_PROFILE_BEGIN_SESSION("Startup", "UglyProfile-Startup.json");
    auto app = Ugly::CreateApplication();
    UE_PROFILE_END_SESSION();

    UE_PROFILE_BEGIN_SESSION("Runtime", "UglyProfile-Runtime.json");
    app->Run();
    UE_PROFILE_END_SESSION();

    UE_PROFILE_BEGIN_SESSION("Shutdown", "UglyProfile-Shutdown.json");
    delete app;
    UE_PROFILE_END_SESSION();
}

#endif
