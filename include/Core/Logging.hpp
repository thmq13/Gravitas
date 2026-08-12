#pragma once

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <string>
#include <deque>
#include <fstream>
#include <utility>
#include <format>
#include <variant>
#include <mutex>
#include <cstdlib>

#include "Utils/ToString.hpp"

#define GVT_LOG_IMPL(level, ...) \
  do { ::gravitas::core::Log(::gravitas::core::LogLevel::level, __VA_ARGS__); } while (0)

#define GVT_DEBUG(...) GVT_LOG_IMPL(Debug, __VA_ARGS__)
#define GVT_TRACE(...) GVT_LOG_IMPL(Trace, __VA_ARGS__)
#define GVT_INFO(...)  GVT_LOG_IMPL(Info,  __VA_ARGS__)
#define GVT_WARN(...)  GVT_LOG_IMPL(Warn,  __VA_ARGS__)
#define GVT_ERROR(...) GVT_LOG_IMPL(Error, __VA_ARGS__)
#define GVT_FATAL(...) do { GVT_LOG_IMPL(Fatal, __VA_ARGS__); std::abort(); } while(0)
#define GVT_ASSERT(condition, ...) do { assert(condition && __VA_ARGS__); } while(0)

namespace gravitas::core
{
  enum class LogLevel : std::uint8_t {
    Debug = 0,
    Trace,
    Info,
    Warn,
    Error,
    Fatal
  };

  struct LogMessage {
    LogLevel level{};
    std::string text{};
  };

  class Logger {
  public:
    static constexpr std::size_t kMaxMessageCount{ 1000 };

    [[nodiscard]] static Logger& GetInstance();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) noexcept = delete;
    Logger& operator=(Logger&&) noexcept = delete;

    void Submit(LogLevel level, std::string message);
    void Draw();
    void Clear();

  private:
    Logger();
    ~Logger() = default;

    void OpenLogFile();

    std::deque<LogMessage> m_messages{};
    std::ofstream m_logFile{};
    bool m_autoScroll{ true };
    mutable std::mutex m_mutex{};
  };

  template <typename... Args>
  void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
    std::string formatted_message{ std::format(fmt, std::forward<Args>(args)...) };
    Logger::GetInstance().Submit(level, std::move(formatted_message));
  }
} // namespace gravitas::core

template <typename... Types>
struct std::formatter<std::variant<Types...>, char> {
  constexpr auto parse(std::format_parse_context& context) {
    return context.begin();
  }
  auto format(const std::variant<Types...>& v, std::format_context& context) const {
    return std::visit([&context](const auto& arg) {
      return std::format_to(context.out(), "{}", arg);
    }, v);
  }
};

template <gravitas::utils::ConvertibleToStringView ConvertibleType>
struct std::formatter<ConvertibleType, char> {
  constexpr auto parse(std::format_parse_context& context) {
    return context.begin();
  }
  auto format(const ConvertibleType& v, std::format_context& context) const {
    return std::format_to(context.out(), "{}", gravitas::utils::ToString(v));
  }
};
