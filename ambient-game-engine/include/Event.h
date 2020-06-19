#pragma once

// Included in Precompiled Header. Can remove?
#include <sstream>
#include <string>

#define BIT(x) (1 << x)

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static Type GetStaticType()                                                                                        \
    {                                                                                                                  \
        return Ambient::Event::Type::type;                                                                             \
    }                                                                                                                  \
    virtual Type GetEventType() const override                                                                         \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const char *GetName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    virtual int GetCategoryFlags() const override                                                                      \
    {                                                                                                                  \
        return category;                                                                                               \
    };

namespace Ambient
{
namespace Event
{
enum class Type
{ // EventType is a strict type
    None = 0,

    // Window Events
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,

    // App Events
    AppTick,
    AppUpdate,
    AppRender,

    // Keyboard events
    KeyPressed,
    KeyReleased,

    // Mouse events
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum Catergory
{
    None = 0,
    EventCatergoryApplication = BIT(0),
    EventCatergoryInput = BIT(1),
    EventCatergoryKeyboard = BIT(2),
    EventCatergoryMouse = BIT(3),
    EventCatergoryMouseButton = BIT(4),
};

class Event
{
    friend class EventDispatcher;

  public:
    bool Handled = false;

    // Pure virtual functions: **Have** to be overidden in the derived class
    virtual Ambient::Event::Type GetEventType() const = 0;
    virtual const char *GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;

    virtual std::string ToString() const
    {
        return GetName();
    }

    inline bool IsInCategory(Ambient::Event::Catergory category)
    {
        return GetCategoryFlags() & category; // bitwise &
    }
};

class WindowResizeEvent : public Event
{
  protected:
    int m_Width, m_Height;

  public:
    WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height)
    {
    }
    inline unsigned int GetWidth() const
    {
        return m_Width;
    }
    inline unsigned int GetHeight() const
    {
        return m_Height;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCatergoryApplication)
};

class WindowCloseEvent : public Event
{
  public:
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowCloseEvent";
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCatergoryApplication)
};

class KeyEvent : public Event
{
  protected:
    int m_KeyCode;
    KeyEvent(int keycode) : m_KeyCode(keycode)
    {
    }

  public:
    inline unsigned int GetKeyCode() const
    {
        return m_KeyCode;
    }
    EVENT_CLASS_CATEGORY(EventCatergoryInput | EventCatergoryKeyboard)
};

class KeyPressedEvent : public KeyEvent
{
    int m_RepeatCount;

  public:
    KeyPressedEvent(int keycode, int repeat_count) : KeyEvent(keycode), m_RepeatCount(repeat_count)
    {
    }

    inline unsigned int GetRepeatCount() const
    {
        return m_RepeatCount;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
    EVENT_CLASS_CATEGORY(EventCatergoryInput | EventCatergoryKeyboard)
};

/*
 * Mouse Events
 * 1. MouseButtonPressed
 * 2. MouseButtonReleased
 */

class MouseButtonEvent : public Event
{
  protected:
    int m_Button;

  public:
    MouseButtonEvent(int button) : m_Button(button)
    {
    }

    EVENT_CLASS_CATEGORY(EventCatergoryInput | EventCatergoryMouse | EventCatergoryMouseButton)
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed);
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased);
};

class MousePositionEvent : public Event
{
  protected:
    double m_X, m_Y;

  public:
    MousePositionEvent(double x, double y) : m_X(x), m_Y(y)
    {
    }

    EVENT_CLASS_CATEGORY(EventCatergoryInput | EventCatergoryMouse);
};

class MouseMovedEvent : public MousePositionEvent
{
  public:
    MouseMovedEvent(double x, double y) : MousePositionEvent(x, y)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_X << ", " << m_Y;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
};

class MouseScrolledEvent : public MousePositionEvent
{
  public:
    MouseScrolledEvent(double x, double y) : MousePositionEvent(x, y)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << m_X << ", " << m_Y;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
};

class EventDispatcher
{
    template <typename T> using EventFn = std::function<bool(T &)>;

    Event &m_Event;

  public:
    EventDispatcher(Event &event) : m_Event(event)
    {
    }

    template <typename T> bool Dispatch(EventFn<T> func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.Handled = func(*(T *)&m_Event);
            return true;
        }
        return false;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Event &e)
{
    return os << e.ToString();
}

} // namespace Event
} // namespace Ambient
