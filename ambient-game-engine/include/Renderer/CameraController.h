#pragma

#include "Event.h"
#include "Renderer/Camera.h"
#include "Timestep.h"

#include <glm/glm.hpp>

namespace Ambient
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectratio, bool rotation = false);

        virtual ~OrthographicCameraController() = default;

        void OnUpdate(Timestep ts);

        void OnEvent(Event& e);

        inline OrthographicCamera& GetCamera()
        {
            return m_Camera;
        }

        inline const OrthographicCamera& GetCamera() const
        {
            return m_Camera;
        }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);

        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_Rotation;

        OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;

        float m_CameraTranslationSpeed = 4.0f;
        float m_CameraRotationSpeed = 180.0f;
    };
} // namespace Ambient
