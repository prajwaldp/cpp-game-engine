#pragma once

namespace Ambient
{
    class TimeStep
    {
    public:
        explicit TimeStep(float time = 0.0f) : m_Time(time)
        {
        }

        [[nodiscard]] float GetSeconds() const
        {
            return m_Time;
        }

        [[nodiscard]] float GetMilliSeconds() const
        {
            return m_Time * 1000.0f;
        }

        operator float() const
        {
            return m_Time;
        }

    private:
        float m_Time; // time in seconds
    };
} // namespace Ambient
