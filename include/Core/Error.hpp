#pragma once

#include <cstdint>
#include <cassert>

#include "Core/Logging.hpp"

#define GVT_ENSURE_INIT(isInitialized, className) \
  do { \
    assert((isInitialized) && className " called before initialization"); \
    if (!(isInitialized)) { \
      GVT_ERROR("{}::{} called before initialization", className, __func__); \
      return; \
    } \
  } while (0)

namespace gravitas::core
{
  enum class AppError : std::uint8_t {
    NotInitialized = 0,
    AlreadyInitialized,
    RenderEngineFailed
  };
} // namespace gravitas::core

namespace gravitas::render
{
  enum class RenderEngineError : std::uint8_t {
    NotInitialized = 0,
    AlreadyInitialized,
    ParticleBufferNotFound,
    WindowManagerFailed
  };

  enum class WindowError : std::uint8_t {
    NotInitialized = 0,
    AlreadyInitialized,
    WindowCreationFailed
  };

} // namespace gravitas::render