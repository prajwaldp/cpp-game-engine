#pragma once

#include "Renderer/Camera.h"

namespace Ambient
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio);

    private:
        float m_aspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;
    };

} // namespace Ambient
