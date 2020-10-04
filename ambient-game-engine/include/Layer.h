#pragma once

#include "Event.h"
#include "TimeStep.h"

namespace Ambient
{
    class Layer
    {
    protected:
        std::string m_DebugName;

    public:
        Layer(const std::string& name = "Layer");

        virtual ~Layer();

        // When a layer is pushed into the layer stack
        virtual void OnAttach()
        {
        }

        // When a layer is popped from the layer stack
        virtual void OnDetach()
        {
        }

        // When a layer is updated
        virtual void OnUpdate(TimeStep ts)
        {
        }

        // When an event is sent to a layer
        virtual void OnEvent(Event& event)
        {
        }

        inline const std::string& GetName() const
        {
            return m_DebugName;
        }
    };
} // namespace Ambient
