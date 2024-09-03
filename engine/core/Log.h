#pragma once
#include <Core.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace UglyEngine {
 
    class UGLY_ENGINE_API Log {
        public:
           static void Init();

           inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { 
               return s_CoreLogger; 
           }
           inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { 
               return s_ClientLogger; 
           }
        private:
           static std::shared_ptr<spdlog::logger> s_CoreLogger;
           static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core Log Macros
#define UE_CORE_TRACE(...) ::UglyEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define UE_CORE_INFO(...) ::UglyEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define UE_CORE_WARN(...) ::UglyEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define UE_CORE_ERROR(...) ::UglyEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define UE_CORE_CRITICAL(...) ::UglyEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// ClienU Log Macros
#define UE_TRACE(...)    ::UglyEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define UE_INFO(...)     ::UglyEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define UE_WARN(...)     ::UglyEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define UE_ERROR(...)    ::UglyEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define UE_CRITICAL(...) ::UglyEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
