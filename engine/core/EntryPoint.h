#pragma once

#ifdef __linux__

extern UglyEngine::Application* UglyEngine::CreateApplication();

int main(int argc, char** argv){
    UglyEngine::Log::Init();
    UE_CORE_WARN("initialized core log");
    int a = 5;
    UE_INFO("initialized client log, Var={0}", a);

    auto app = UglyEngine::CreateApplication();
    app->Run();
    delete app;
}

#endif
