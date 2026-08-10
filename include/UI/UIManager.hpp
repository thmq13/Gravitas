#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "Core/EventBus.hpp"
#include "Core/AppState.hpp"

namespace gravitas::ui 
{
  class UIPanel;

  class UIManager : public core::EventBusSubscriber<UIManager> {
  public:
    explicit UIManager(core::EventBus& bus);

    ~UIManager();

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    UIManager(UIManager&&) noexcept = delete;
    UIManager& operator=(UIManager&&) noexcept = delete;

    void Draw(core::AppState state);
    void HandleEvent(const core::Event& event);

  private:
    void RegisterPanels();

    core::EventBus& m_bus;

    using PanelVector = std::vector<std::unique_ptr<UIPanel>>;
    using PanelMap    = std::unordered_map<core::AppState, PanelVector>;
    PanelMap m_panels{};
  };
} // namespace gravitas::ui 