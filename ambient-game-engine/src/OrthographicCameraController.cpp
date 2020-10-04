#include "Input.h"
#include "KeyCodes.h"
#include "Renderer/CameraController.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ambient
{

    OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool rotation)
            : m_AspectRatio(aspect_ratio),
              m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
              m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(TimeStep ts)
    {
        float time = ts;

        if (Input::IsKeyPressed(AM_KEY_A))
        {
            m_CameraPosition.x -= m_CameraTranslationSpeed * time;
        }
        else if (Input::IsKeyPressed(AM_KEY_D))
        {
            m_CameraPosition.x += m_CameraTranslationSpeed * time;
        }

        if (Input::IsKeyPressed(AM_KEY_W))
        {
            m_CameraPosition.y += m_CameraTranslationSpeed * time;
        }
        else if (Input::IsKeyPressed(AM_KEY_S))
        {
            m_CameraPosition.y -= m_CameraTranslationSpeed * time;
        }

        m_Camera.SetPosition(m_CameraPosition);

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(AM_KEY_Q))
            {
                m_CameraRotation += m_CameraRotationSpeed * time;
            }
            else if (Input::IsKeyPressed(AM_KEY_E))
            {
                m_CameraRotation -= m_CameraRotationSpeed * time;
            }

            m_Camera.SetRotation(m_CameraRotation);
        }
    }

    void OrthographicCameraController::CalculateView()
    {
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        m_CameraTranslationSpeed = m_ZoomLevel;
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

} // namespace Ambient
