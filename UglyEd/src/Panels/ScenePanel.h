#pragma once

#include "Log.h"
#include "Scene.h"
#include "Entity.h"

namespace Ugly {
    class ScenePanel {
        public:
            ScenePanel() = default;
            ScenePanel(const Ref<Scene>& context);

            void SetContext(const Ref<Scene>& context);
            void OnImGuiRender();
        private:
            void DrawEntityNode(Entity entity);
            void DrawComponents(Entity entity);
        private:
            Ref<Scene> m_Context;
            Entity m_SelectionContext;
    };
}
