#include "Render/RenderEngine.hpp"

#include <cassert>
#include <variant>
#include <type_traits>
#include <utility>

#include <raylib.h>

#include "Core/Logging.hpp"

namespace gravitas::render
{
  namespace
  {
    [[nodiscard]] constexpr bool ShouldDraw3D(core::AppState state) noexcept {
      return state == core::AppState::PrecomputeConfig  ||
             state == core::AppState::PrecomputeRunning ||
             state == core::AppState::RealTimeConfig    ||
             state == core::AppState::RealTimeRunning   ||
             state == core::AppState::PlaybackConfig    ||
             state == core::AppState::PlaybackRunning;
    }
  } // namespace

  RenderEngine::RenderEngine(core::EventBus& bus, std::shared_ptr<particle::ParticleBuffer> sharedBuffer) noexcept
    : m_bus(bus) 
    , m_particleBuffer(std::move(sharedBuffer)) {}

  RenderEngine::~RenderEngine() {
    GVT_INFO("RenderEngine::~RenderEngine freeing resources");
  }

  std::expected<void, render::RenderEngineError> RenderEngine::Initialize() {
    assert(!m_isInitialized && "RenderEngine::Initialize called on an already initialized render engine");
    if (m_isInitialized) {
      return std::unexpected(RenderEngineError::AlreadyInitialized);
    }

    if (!m_particleBuffer) {
      return std::unexpected(RenderEngineError::ParticleBufferNotFound);
    }

    if (auto result{ m_windowManager.Initialize() }; !result) {
      GVT_ERROR("RenderEngine::Initialize failed to initialize window manager: {}", result.error()); 
      return std::unexpected(RenderEngineError::WindowManagerFailed);
    }

    m_isInitialized = true;

    GVT_INFO("RenderEngine::Initialize succeeded");

    return {};
  } 

  void RenderEngine::HandleEvent(const core::Event& event) {
    GVT_ENSURE_INIT(m_isInitialized, "RenderEngine");
    
    std::visit([this](const auto& evt) {
      using EventType = std::decay_t<decltype(evt)>;

      // if constexpr (std::same_as<EventType, core::EvtPropertyChange>) {
      //   //...
      // }
    }, event);
  }

  void RenderEngine::BeginFrame() {
    m_windowManager.BeginFrame();   
  }

  void RenderEngine::EndFrame() {
    m_windowManager.EndFrame();
  }

  void RenderEngine::Render(core::AppState state) {
    GVT_ENSURE_INIT(m_isInitialized, "RenderEngine");

    if (!ShouldDraw3D(state)) {
      return;
    }
    m_windowManager.Begin3D();
    ::DrawGrid(20, 1.0f);
    m_windowManager.End3D();
  }

  bool RenderEngine::ShouldClose() const {
    if (!m_isInitialized) {
      return false;
    }
    return m_windowManager.ShouldClose();
  }
} // namespace gravitas::render
