#pragma once

#include <cstdint>
#include <expected>

#include <raylib.h>

#include "Core/EventBus.hpp"
#include "Core/Error.hpp"

namespace gravitas::render
{
  class WindowManager {
  public:
    WindowManager() noexcept;
    
    ~WindowManager();

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
    WindowManager(WindowManager&&) noexcept = delete;
    WindowManager& operator=(WindowManager&&) noexcept = delete;

    [[nodiscard]] std::expected<void, WindowError> Initialize();
    void BeginFrame();
    void EndFrame();
    void Begin3D();
    void End3D();
    [[nodiscard]] bool ShouldClose() const;

  private:
    void UpdateCamera();

    bool m_isInitialized{ false };
    bool m_isFrameActive{ false };
    bool m_isRlImGuiInitialized{ false };

    std::uint32_t m_framesPerSecond{ 60 };
    bool m_isMovementLock{ true };

    Camera3D m_camera{};
  };
} // namespace gravitas::render