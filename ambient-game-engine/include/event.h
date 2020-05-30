#pragma once

#include <sstream>
#include <string>

#define BIT(x) (1 << x)

#define EVENT_CLASS_TYPE(type)                                           \
  static Type GetStaticType() { return Ambient::Event::Type::type; }     \
  virtual Type GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; };

namespace Ambient {
namespace Event {
enum class Type {  // EventType is a strict type
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

enum Catergory {
  None = 0,
  EventCatergoryApplication = BIT(0),
  EventCatergoryInput = BIT(1),
  EventCatergoryKeyboard = BIT(2),
  EventCatergoryMouse = BIT(3),
  EventCatergoryMouseButton = BIT(4),
};

class Event {
  friend class EventDispatcher;

 protected:
  bool m_Handled = false;

 public:
  // Pure virtual functions: **Have** to be overidden in the derived class
  virtual Ambient::Event::Type GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;

  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(Ambient::Event::Catergory category) {
    return GetCategoryFlags() & category;  // bitwise &
  }
};

class WindowResizeEvent : public Event {
 protected:
  int m_Width, m_Height;

 public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : m_Width(width), m_Height(height) {}
  inline unsigned int GetWidth() const { return m_Width; }
  inline unsigned int GetHeight() const { return m_Height; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCatergoryApplication)
};

class KeyEvent : public Event {
 protected:
  int m_KeyCode;
  KeyEvent(int keycode) : m_KeyCode(keycode) {}

 public:
  inline unsigned int GetKeyCode() const { return m_KeyCode; }
  EVENT_CLASS_CATEGORY(EventCatergoryInput | EventCatergoryKeyboard)
};

class KeyPressedEvent : public KeyEvent {
  int m_RepeatCount;

 public:
  KeyPressedEvent(int keycode, int repeat_count)
      : KeyEvent(keycode), m_RepeatCount(repeat_count) {}

  inline unsigned int GetRepeatCount() const { return m_RepeatCount; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)
};

class EventDispatcher {
  template <typename T>
  using EventFn = std::function<bool(T&)>;

  Event& m_Event;

 public:
  EventDispatcher(Event& event) : m_Event(event) {}

  template <typename T>
  bool Dispatch(EventFn<T> func) {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      m_Event.m_Handled = func(*(T*)&m_Event);
      return true;
    }
    return false;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}

}  // namespace Event
}  // namespace Ambient
