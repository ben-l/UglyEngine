#pragma once
#include <memory>

#ifdef __linux__ 
    // not needed for static library
    //#define UGLY_ENGINE_API __attribute__((visibility("default")))
    #define UGLY_ENGINE_API
#else
#error Ugly only supports Linux!
#endif

#ifdef UE_DEBUG
    #define UE_ENABLE_ASSERTS
#endif

#ifdef UE_ENABLE_ASSERTS
    #define UE_ASSERT(x, ...) { if(!(x)) { \
            UE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak(); } }
    #define UE_CORE_ASSERT(x, ...) { if(!(x)) { \
            UE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak(); } }
#else
    #define UE_ASSERT(x, ...)
    #define UE_CORE_ASSERT(x, ...)
#endif


// binary - shift 1 by x places to the left
// so 1 added by (x) 0s e.g. BIT(4) == 1000
// converted to decimal is 16
#define BIT(x) (1 << x)

#define UE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Ugly {
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

}
