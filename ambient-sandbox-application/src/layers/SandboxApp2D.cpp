#include "layers/SandboxApp2D.h"

SandboxApp2D::SandboxApp2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void SandboxApp2D::OnAttach()
{
    m_Texture = Ambient::Texture2D::Create("../../assets/batthern.png");
}

void SandboxApp2D::OnDetach()
{
}

void SandboxApp2D::OnUpdate(Ambient::Timestep ts)
{
    float time = ts;
    m_CameraController.OnUpdate(ts);

    Ambient::RenderCommand::SetClearColor();
    Ambient::RenderCommand::Clear();

    static float rotation = 0.0f;
    rotation -= ts * 180.0f;

    Ambient::Renderer2D::ResetStats();
    Ambient::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Ambient::Renderer2D::DrawQuad(
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.2f, 0.8f, 0.3f, 1.0f }
    );

    Ambient::Renderer2D::DrawRotatedQuad(
            { 0.0f, 0.0f },
            { 1.0f, 1.0f },
            rotation,
            { 0.8f, 0.2f, 0.3f, 1.0f }
    );

    Ambient::Renderer2D::DrawRotatedQuad(
            { 0.0f, 0.0f, 0.5f },
            { 1.0f, 1.0f },
            rotation,
            m_Texture
    );

    Ambient::Renderer2D::EndScene();
}

void SandboxApp2D::OnEvent(Ambient::Event& e)
{
    m_CameraController.OnEvent(e);
}
