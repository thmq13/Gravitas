#include "Core/App.hpp"

#include <cassert>
#include <variant>
#include <type_traits>

#include "Core/Logging.hpp"
#include "Particle/ParticleBuffer.hpp"

namespace gravitas::core
{
  App::App() 
  : m_particleBuffer(std::make_shared<particle::ParticleBuffer>()) 
  , m_renderEngine(m_bus, m_particleBuffer) {}

  App::~App() {
    if (m_isRunning) {
      RequestShutdown();
    }
    GVT_INFO("App::~App freeing resources");
  }

  std::expected<void, AppError> App::Initialize() {
    assert(!m_isInitialized && "App::Initialize called on an already initialized application");
    if (m_isInitialized) {
      return std::unexpected(AppError::AlreadyInitialized);
    }

    if (auto result{ m_renderEngine.Initialize() }; !result) {
      GVT_ERROR("App::Initialize failed to initialize render engine: {}", result.error()); 
      return std::unexpected(AppError::RenderEngineFailed);
    }

    SubscribeToEvents<EvtRequestShutdown, EvtRequestStateChange>(m_bus);

    m_isRunning     = true;
    m_isInitialized = true;

    GVT_INFO("App::Initialize succeeded");

    ChangeState(AppState::TitleScreen);

    return {};
  }

  std::expected<void, AppError> App::Loop() {
    assert(m_isInitialized && "App::Loop called before App::Initialize");
    if (!m_isInitialized) {
      return std::unexpected(AppError::NotInitialized);
    }

    GVT_INFO("App::Loop entering the master loop");
    while (m_isRunning) {
      m_bus.Dispatch();
      if (m_renderEngine.ShouldClose()) {
        RequestShutdown();
        break;
      }
      m_renderEngine.BeginFrame();
      m_renderEngine.Render(m_state);
      m_uiManager.Draw(m_state);
      m_renderEngine.EndFrame();
    }
    return {};
  }

  void App::HandleEvent(const Event& event) {
    assert(m_isInitialized && "App::HandleEvent called before App::Initialize");
    if (!m_isInitialized) {
      GVT_ERROR("App::HandleEvent called before App::Initialize");
      return;
    }

    std::visit([this](const auto& evt) {
      using EventType = std::decay_t<decltype(evt)>;

      if constexpr (std::same_as<EventType, EvtRequestShutdown>) {
        RequestShutdown();
      } else if constexpr (std::same_as<EventType, EvtRequestStateChange>) {
        ChangeState(evt.requestedState);
      }
    }, event);
  }

  void App::RequestShutdown() noexcept {
    GVT_INFO("App::RequestShutdown executing");
    ChangeState(AppState::TearDown);
    m_isRunning = false;
  }

  void App::ChangeState(AppState newState) noexcept {
    if (newState == m_state) {
      GVT_WARN("App::ChangeState did nothing because '{}' is already the current state", newState);
      return;
    }

    m_state = newState;
    GVT_INFO("App::ChangeState changed state to '{}'", m_state);
  }
} // namespace gravitas::core
