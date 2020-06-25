//
// Created by Prajwal Prakash on 25/06/20.
//

#ifndef AMBIENT_GAMEOBJECT_H
#define AMBIENT_GAMEOBJECT_H

#include <glm/glm.hpp>
#include <Types.h>

#include "Component.h"


namespace Ambient
{
    class GameObject
    {
    public:
        GameObject() = default;

        ~GameObject() = default;

        void AddComponent(Ref<Component>& component);

        Ref<Component> GetComponent(const std::string& type);

        Ref<Component> GetComponentLike(Ref<Component>& other);

    private:
        bool m_IsActive = true;
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        std::vector<Ref<Component>> m_Components = {};
    };
}

#endif //AMBIENT_GAMEOBJECT_H
