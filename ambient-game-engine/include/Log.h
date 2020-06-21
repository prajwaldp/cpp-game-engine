#pragma once

#include <memory>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Ambient
{
    class Logger
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        };

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        };

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
} // namespace Ambient

// Core Log Macros
#define AM_CORE_TRACE(...) ::Ambient::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define AM_CORE_INFO(...) ::Ambient::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define AM_CORE_WARN(...) ::Ambient::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define AM_CORE_ERROR(...) ::Ambient::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define AM_CORE_FATAL(...) ::Ambient::Logger::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log Macros
#define AM_TRACE(...) ::Ambient::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define AM_INFO(...) ::Ambient::Logger::GetClientLogger()->info(__VA_ARGS__)
#define AM_WARN(...) ::Ambient::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define AM_ERROR(...) ::Ambient::Logger::GetClientLogger()->error(__VA_ARGS__)
#define AM_FATAL(...) ::Ambient::Logger::GetClientLogger()->fatal(__VA_ARGS__)
