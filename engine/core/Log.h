#pragma once
#include <Core.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Ugly {
 
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
#define UE_CORE_TRACE(...) ::Ugly::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define UE_CORE_INFO(...) ::Ugly::Log::GetCoreLogger()->info(__VA_ARGS__)
#define UE_CORE_WARN(...) ::Ugly::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define UE_CORE_ERROR(...) ::Ugly::Log::GetCoreLogger()->error(__VA_ARGS__)
#define UE_CORE_CRITICAL(...) ::Ugly::Log::GetCoreLogger()->critical(__VA_ARGS__)

// ClienU Log Macros
#define UE_TRACE(...)    ::Ugly::Log::GetClientLogger()->trace(__VA_ARGS__)
#define UE_INFO(...)     ::Ugly::Log::GetClientLogger()->info(__VA_ARGS__)
#define UE_WARN(...)     ::Ugly::Log::GetClientLogger()->warn(__VA_ARGS__)
#define UE_ERROR(...)    ::Ugly::Log::GetClientLogger()->error(__VA_ARGS__)
#define UE_CRITICAL(...) ::Ugly::Log::GetClientLogger()->critical(__VA_ARGS__)
