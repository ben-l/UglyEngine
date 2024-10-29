#pragma once

#include "Core.h"
#include "Scene.h"
#include "Entity.h"
#include "Log.h"

namespace Ugly {
    class ScenePanel {
        public:
            ScenePanel() = default;
            ScenePanel(const Ref<Scene>& context);

            void SetContext(const Ref<Scene>& context);
            void OnImGuiRender();

            Entity GetSelectedEntity() const { return m_SelectionContext; }
        private:
            void DrawEntityNode(Entity entity);
            void DrawComponents(Entity entity);
        private:
            Ref<Scene> m_Context;
            Entity m_SelectionContext;
    };
}
