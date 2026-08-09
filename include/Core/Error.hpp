#pragma once

#include <cstdint>

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