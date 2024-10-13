#include "uepch.h"
#include "Scene.h"

#include "Components.h"
#include "Renderer2d.h"

#include <glm/glm.hpp>

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

    entt::entity Scene::CreateEntity(){
        return m_Registry.create();
    }

    void Scene::OnUpdate(Timestep ts){
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for(auto entity : group){
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2d::DrawQuad(transform, sprite.Color);
        }
    }
};
