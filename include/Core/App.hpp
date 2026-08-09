#pragma once

#include <expected>
#include <memory>

#include "Core/AppState.hpp"
#include "Core/EventBus.hpp"
#include "Core/Error.hpp"
#include "UI/UIManager.hpp"
#include "Render/RenderEngine.hpp"

namespace gravitas::particle { class ParticleBuffer; }

namespace gravitas::core
{
  class App : public EventBusSubscriber<App> {
  public:
    App();
    
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) noexcept = delete;
    App& operator=(App&&) noexcept = delete;

    [[nodiscard]] std::expected<void, AppError> Initialize(); 
    [[nodiscard]] std::expected<void, AppError> Loop();

    void HandleEvent(const Event& event);
    void RequestShutdown() noexcept;

  private:
    void ChangeState(AppState newState) noexcept;

    bool                                      m_isInitialized{ false };
    bool                                      m_isRunning{ false };
    AppState                                  m_state{ AppState::Uninitialized };
    EventBus                                  m_bus{};
    std::shared_ptr<particle::ParticleBuffer> m_particleBuffer{};
    ui::UIManager                             m_uiManager{ m_bus };
    render::RenderEngine                      m_renderEngine;
  };
} // namespace gravitas::core
