//
// Created by Prajwal Prakash on 25/06/20.
//

#ifndef AMBIENT_COMPONENT_H
#define AMBIENT_COMPONENT_H

#include "TimeStep.h"
#include "Types.h"


namespace Ambient
{
    class Component
    {
    public:
        static Ref<Component> Create();
        void OnUpdate(TimeStep ts);
    };
}


#endif //AMBIENT_COMPONENT_H
