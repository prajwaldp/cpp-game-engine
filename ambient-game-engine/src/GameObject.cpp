//
// Created by Prajwal Prakash on 25/06/20.
//

#include "GameObject.h"
#include "Log.h"

namespace Ambient
{
    void GameObject::AddComponent(Ref<Component>& component)
    {
        for (auto& existing : m_Components)
        {
            if (typeid(existing).name() == typeid(component).name())
                AM_CORE_ERROR("Cannot add component of type {} to the game object",
                        typeid(component).name());
            return;
        }

        m_Components.push_back(component);
    }

    Ref<Component> GameObject::GetComponent(const std::string& type)
    {
        for (auto& component : m_Components)
        {
            if (typeid(component).name() == type) return component;
        }

        AM_CORE_ERROR("Cannot find component with type {}", type);

        return nullptr;
    }

    Ref<Component> GameObject::GetComponentLike(Ref<Component>& other)
    {
        for (auto& component : m_Components)
        {
            if (typeid(component).name() == typeid(other).name()) return component;
        }

        AM_CORE_ERROR("Cannot find component with type {}", typeid(other).name());

        return nullptr;
    }
}
