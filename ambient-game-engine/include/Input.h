#pragma once

namespace Ambient
{
class Input
{
  public:
    static bool IsKeyPressed(int keycode)
    {
        return s_Instance->IsKeyPressedImpl(keycode);
    }

  protected:
    virtual bool IsKeyPressedImpl(int keycode) = 0;

  private:
    static Input* s_Instance;
};

class MacOSInput : public Input
{
  protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
};
} // namespace Ambient
