#include "Render/WindowManager.hpp"

#include <cassert>

#include <imgui.h>
#include <rlImGui.h>

#include "Core/Logging.hpp"

namespace gravitas::render
{
  namespace
  {
    constexpr float kMovementSpeed{ 15.0f };
    constexpr float kFasterMovementSpeed{ kMovementSpeed * 2.5f };
    constexpr float kPanSpeed{ 5.0f };
  } // namespace

  WindowManager::WindowManager() noexcept {
    m_camera = {
      .position   = {0.0f, 25.0f, 35.0f},
      .target     = {0.0f, 0.0f, 0.0f},
      .up         = {0.0f, 1.0f, 0.0f},
      .fovy       = {70.0f},
      .projection = CAMERA_PERSPECTIVE
    };
  }

  WindowManager::~WindowManager() {
    if (m_isRlImGuiInitialized) {
      if (IsWindowReady()) {
        rlImGuiShutdown();
      }
      m_isRlImGuiInitialized = false;
    }
    if (IsWindowReady()) {
      CloseWindow();
    }
    GVT_INFO("WindowManager::~WindowManager freeing resources");
  }

  std::expected<void, WindowError> WindowManager::Initialize() {
    assert(!m_isInitialized && "WindowManager::Initialize called on an already initialized window manager");
    if (m_isInitialized) {
      return std::unexpected(WindowError::AlreadyInitialized);
    }

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1280, 720, "Gravitas");
    if (!IsWindowReady()) {
      return std::unexpected(WindowError::WindowCreationFailed);
    }

    int monitor_width{ GetMonitorWidth(0) };
    int monitor_height{ GetMonitorHeight(0) };

    int padding_width{ monitor_width / 4 };
    int padding_height{ monitor_height / 4 };

    SetWindowSize(monitor_width - padding_width, monitor_height - padding_height);
    SetWindowPosition(padding_width / 2, padding_height / 2);
    SetTargetFPS(static_cast<int>(m_framesPerSecond));

    rlImGuiSetup(true);
    m_isRlImGuiInitialized = true;

    m_isInitialized = true;

    GVT_INFO("WindowManager::Initialize succeeded");

    return {}; 
  }

  void WindowManager::BeginFrame() {
    assert(m_isInitialized && "WindowManager::BeginFrame called before WindowManager::Initialize");
    if (!m_isInitialized) {
      GVT_ERROR("WindowManager::BeginFrame called before WindowManager::Initialize");
      return;
    }

    assert(!m_isFrameActive && "WindowManager::BeginFrame called while a frame is already active");
    if (m_isFrameActive) {
      GVT_ERROR("WindowManager::BeginFrame called while a frame is already active");
      return;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    rlImGuiBegin();
    m_isFrameActive = true;
  }

  void WindowManager::EndFrame() {
    assert(m_isInitialized && "WindowManager::EndFrame called before WindowManager::Initialize");
    if (!m_isInitialized) {
      GVT_ERROR("WindowManager::EndFrame called before WindowManager::Initialize");
      return;
    }  

    assert(m_isFrameActive && "WindowManager::EndFrame called without matching BeginFrame");
    if (!m_isFrameActive) {
      GVT_ERROR("WindowManager::EndFrame called without matching BeginFrame");
      return;
    }

    rlImGuiEnd();
    EndDrawing();
    m_isFrameActive = false;
  }

  void WindowManager::Begin3D() {
    assert(m_isInitialized && "WindowManager::Begin3D called before WindowManager::Initialize");
    if (!m_isInitialized) {
      GVT_ERROR("WindowManager::Begin3D called before WindowManager::Initialize");
      return;
    }

    assert(m_isFrameActive && "WindowManager::Begin3D called outside of an active frame");
    if (!m_isFrameActive) {
      GVT_ERROR("WindowManager::Begin3D called outside of an active frame");
      return;
    }

    UpdateCamera();
    BeginMode3D(m_camera);
  }

  void WindowManager::End3D() {
    assert(m_isInitialized && "WindowManager::End3D called before WindowManager::Initialize");
    if (!m_isInitialized) {
      GVT_ERROR("WindowManager::End3D called before WindowManager::Initialize");
      return;
    }

    assert(m_isFrameActive && "WindowManager::End3D called outside of an active frame");
    if (!m_isFrameActive) {
      GVT_ERROR("WindowManager::End3D called outside of an active frame");
      return;
    }

    EndMode3D();
  }

  bool WindowManager::ShouldClose() const {
    if (!IsWindowReady()) {
      return false;
    }
    return WindowShouldClose();
  }

  void WindowManager::UpdateCamera() {
    if (!IsWindowReady() || !m_isRlImGuiInitialized) {
      return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      m_isMovementLock = !m_isMovementLock;
      if (m_isMovementLock) {
        EnableCursor();
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
      } else {
        DisableCursor();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
      }
    }

    if (!m_isMovementLock) {
      float   delta_time{ GetFrameTime() };

      float   current_movement_speed{ IsKeyDown(KEY_LEFT_CONTROL) ? kFasterMovementSpeed : kMovementSpeed };
      float   movement_displacement{  current_movement_speed * delta_time };
      Vector3 movement{};
      if (IsKeyDown(KEY_W))          { movement.x += movement_displacement; }
      if (IsKeyDown(KEY_S))          { movement.x -= movement_displacement; }
      if (IsKeyDown(KEY_D))          { movement.y += movement_displacement; }
      if (IsKeyDown(KEY_A))          { movement.y -= movement_displacement; }
      if (IsKeyDown(KEY_SPACE))      { movement.z += movement_displacement; }
      if (IsKeyDown(KEY_LEFT_SHIFT)) { movement.z -= movement_displacement; }

      Vector2 mouse_delta{ GetMouseDelta() };
      float   pan_displacement{ kPanSpeed * delta_time };
      Vector3 rotation{ mouse_delta.x * pan_displacement, mouse_delta.y * pan_displacement };
      
      UpdateCameraPro(&m_camera, movement, rotation, 0.0f);
    }
  }
} // namespace gravitas::render
