#include "uepch.h"
#include "Scene.h"

#include "Components.h"
#include "Renderer2d.h"

#include <glm/glm.hpp>
#include "Entity.h"

namespace Ugly {

    static void Calculate(const glm::mat4& transform){
    }


    Scene::Scene(){
#if ENTT_EXAMPLE_CODE
        entt::entity entity = m_Registry.create();
        m_Registry.emplace_or_replace<TransformComponent>(entity, glm::mat4(1.0f));

        if(m_Registry.any_of<TransformComponent>(entity))
            TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

        auto view = m_Registry.view<TransformComponent>();
        for(auto entity : view){
            TransformComponent& transform = view.get<TransformComponent>(entity);
        }

        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for(auto entity : group){
            //auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
            //Renderer::Submit(mesh, transform);
        }
#endif
    }

    Scene::~Scene(){
    }

    Entity Scene::CreateEntity(const std::string& name){
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(Timestep ts){
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        // Render sprites
        auto view = m_Registry.view<CameraComponent, TransformComponent>();
        for (auto entity : view){
            const auto& [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

            // find main camera in the scene

            if (camera.Primary){
                mainCamera = &camera.Camera;
                cameraTransform = &transform.Transform;
                break;
            }

        }

        // if camera exists, do the rendering
        if (mainCamera){
            Renderer2d::BeginScene(mainCamera->GetProjection(), *cameraTransform);


            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for(auto entity : group){
                const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2d::DrawQuad(transform, sprite.Color);
            }

            Renderer2d::EndScene();

        }

    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height){
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // resize non-fixedAspectRatio Cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view){
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio){
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }
};
