//
// Created by Prajwal Prakash on 25/06/20.
//

#include "Component.h"


namespace Ambient
{
    Ref<Component> Component::Create()
    {
        return std::make_shared<Component>();
    }

    void Component::OnUpdate(TimeStep ts)
    {
    }
}