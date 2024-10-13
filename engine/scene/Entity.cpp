#include "uepch.h"
#include "Entity.h"


namespace Ugly {
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {

    }
};
