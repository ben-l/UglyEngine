#include "uepch.h"
#include "Scene.h"

#include "Components.h"
#include "Renderer2d.h"

#include <glm/glm.hpp>
#include "Entity.h"

namespace Ugly {

    Scene::Scene(){
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

    void Scene::DestroyEntity(Entity entity){
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(Timestep ts){

        // Update scripts
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc){
                if (!nsc.Instance){
                    nsc.Instance = nsc.InstatiateScript();
                    nsc.Instance->m_Entity = Entity{ entity, this};
                    nsc.Instance->OnCreate();
                }
                nsc.Instance->OnUpdate(ts);
            });
        }

        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        // Render sprites
        auto view = m_Registry.view<CameraComponent, TransformComponent>();
        for (auto entity : view){
            const auto [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

            // find main camera in the scene

            if (camera.Primary){
                mainCamera = &camera.Camera;
                cameraTransform = transform.GetTransform();
                break;
            }

        }

        // if camera exists, do the rendering
        if (mainCamera){
            Renderer2d::BeginScene(mainCamera->GetProjection(), cameraTransform);


            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for(auto entity : group){
                const auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2d::DrawQuad(transform.GetTransform(), sprite.Color);
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
    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component){
        static_assert(false);
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component){
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component){
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component){
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component){
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component){
    }

};
