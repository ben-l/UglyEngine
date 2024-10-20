#include <imgui.h>
#include "ScenePanel.h"
#include "Components.h"

namespace Ugly {

    ScenePanel::ScenePanel(const Ref<Scene>& context){
        SetContext(context);
    }

    void ScenePanel::SetContext(const Ref<Scene>& context){
        m_Context = context;
    }

    void ScenePanel::OnImGuiRender(){
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.view<entt::entity>().each([&](auto entityID) {
            Entity entity{ entityID, m_Context.get() };
            DrawEntityNode(entity);
        });

        ImGui::End();
    }


    void ScenePanel::DrawEntityNode(Entity entity){
        auto& tag = entity.GetComponent<TagComponent>();

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());
        if (ImGui::IsItemClicked()){
            m_SelectionContext = entity;
        }
        if (opened){
            ImGui::TreePop();
        }
    }
    
}
