#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
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

        // Deselect Selected Entity
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
            m_SelectionContext = {};
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext){
            DrawComponents(m_SelectionContext);
        }

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

    void ScenePanel::DrawComponents(Entity entity){
        if (entity.HasComponent<TransformComponent>()){
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))){
                tag = std::string(buffer);
            }
        }

        if (entity.HasComponent<TransformComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")){
                auto& transform = entity.GetComponent<TransformComponent>().Transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<CameraComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")){
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;

                ImGui::Checkbox("Primary", &cameraComponent.Primary);

                const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
                const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString)){
                    for (int i = 0; i < 2; i++){
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected)){
                            currentProjectionTypeString = projectionTypeStrings[i];
                            camera.SetProjectionType((SceneCamera::ProjectionType)i);
                        }

                        if (isSelected){
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic){
                    float orthoSize = camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &orthoSize)){
                        camera.SetOrthographicSize(orthoSize);
                    }

                    float orthoNear = camera.GetOrthographicNearClip();
                    if (ImGui::DragFloat("Near", &orthoNear)){
                        camera.SetOrthographicNearClip(orthoNear);
                    }

                    float orthoFar = camera.GetOrthographicFarClip();
                    if (ImGui::DragFloat("Far", &orthoFar)){
                        camera.SetOrthographicFarClip(orthoFar);
                    }

                    ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective){
                    float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
                    if (ImGui::DragFloat("Size", &verticalFOV)){
                        camera.SetPersspectiveVerticalFOV(glm::radians(verticalFOV));
                    }

                    float persNear = camera.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("Near", &persNear)){
                        camera.SetPerspectiveNearClip(persNear);
                    }

                    float persFar = camera.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far", &persFar)){
                        camera.SetPerspectiveFarClip(persFar);
                    }

                }

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<SpriteRendererComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")){
                auto& src = entity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Colour", glm::value_ptr(src.Color));
                ImGui::TreePop();
            }
        }
    }
    
}
