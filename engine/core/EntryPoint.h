#pragma once

#ifdef __linux__

extern Ugly::Application* Ugly::CreateApplication();

int main(int argc, char** argv){
    Ugly::Log::Init();
    UE_CORE_WARN("initialized core log");
    int a = 5;
    UE_INFO("initialized client log, Var={0}", a);

    auto app = Ugly::CreateApplication();
    app->Run();
    delete app;
}

#endif
