#pragma once
#include <Core.h>

namespace Ugly{
    class UGLY_ENGINE_API Input {
        public:
            static bool IsKeyPressed(int keycode); 
            static bool IsMouseButtonPressed(int button);
            static std::pair<float, float> GetMousePosition(); 
            static float GetMouseX();
            static float GetMouseY(); 
    };
}
