#pragma once

#include <string_view>
#include <concepts>

#include "Core/AppState.hpp"
#include "Core/Error.hpp"

namespace gravitas::utils 
{
  constexpr std::string_view ToString(core::AppState state) noexcept {
    switch (state) {
      case core::AppState::Uninitialized:     return "Uninitialized";
      case core::AppState::TitleScreen:       return "TitleScreen";
      case core::AppState::ModeSelection:     return "ModeSelection";
      case core::AppState::RealTimeConfig:    return "RealTimeConfig";
      case core::AppState::RealTimeRunning:   return "RealTimeRunning";
      case core::AppState::PrecomputeConfig:  return "PrecomputeConfig";
      case core::AppState::PrecomputeRunning: return "PrecomputeRunning";
      case core::AppState::PlaybackConfig:    return "PlaybackConfig";
      case core::AppState::PlaybackRunning:   return "PlaybackRunning";
      case core::AppState::TearDown:          return "TearDown";
      default:                                return "Unknown App State";
    }
  }

  constexpr std::string_view ToString(core::AppError error) noexcept {
    switch (error) {
      case core::AppError::NotInitialized:     return "App Not Initialized";
      case core::AppError::AlreadyInitialized: return "App Already Initialized";
      case core::AppError::RenderEngineFailed: return "App Render Engine Failed";
      default:                                 return "Unknown App Error";
    }
  }

  constexpr std::string_view ToString(render::RenderEngineError error) noexcept {
    switch (error) {
      case render::RenderEngineError::NotInitialized:         return "Render Engine Not Initialized";
      case render::RenderEngineError::AlreadyInitialized:     return "Render Engine Already Initialized";
      case render::RenderEngineError::ParticleBufferNotFound: return "Render Engine Particle Buffer Not Found";
      case render::RenderEngineError::WindowManagerFailed:    return "Render Engine Window Manager Failed";
      default:                                                return "Unknown Render Engine Error";
    }
  }

  constexpr std::string_view ToString(render::WindowError error) noexcept {
    switch (error) {
      case render::WindowError::NotInitialized:       return "Window Not Initialized";
      case render::WindowError::AlreadyInitialized:   return "Window Already Initialized";
      case render::WindowError::WindowCreationFailed: return "Window Creation Failed";
      default:                                        return "Unknown Window Error";
    }
  }

  template <typename Type>
  concept ConvertibleToStringView = requires(Type type) {
    { ToString(type) } -> std::convertible_to<std::string_view>;
  };
} // namespace gravitas::utils
