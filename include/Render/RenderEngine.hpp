#pragma once

#include <expected>
#include <memory>

#include "Core/EventBus.hpp"
#include "Core/Error.hpp"
#include "Core/AppState.hpp"
#include "Render/WindowManager.hpp"

namespace gravitas::particle { class ParticleBuffer; }

namespace gravitas::render
{
  class RenderEngine : public core::EventBusSubscriber<RenderEngine> {
  public:
    explicit RenderEngine(core::EventBus& bus, std::shared_ptr<particle::ParticleBuffer> sharedBuffer) noexcept;

    ~RenderEngine();
  
    RenderEngine(const RenderEngine&) = delete;
    RenderEngine& operator=(const RenderEngine&) = delete;
    RenderEngine(RenderEngine&&) noexcept = delete;
    RenderEngine& operator=(RenderEngine&&) noexcept = delete;

    [[nodiscard]] std::expected<void, RenderEngineError> Initialize(); 

    void HandleEvent(const core::Event& event);
    void BeginFrame();
    void EndFrame();
    void Render(core::AppState state);
    [[nodiscard]] bool ShouldClose() const;

  private:
    bool m_isInitialized{ false };
    WindowManager m_windowManager{};
    core::EventBus& m_bus;
    std::shared_ptr<particle::ParticleBuffer> m_particleBuffer{};
  };
} // namespace gravitas::render
