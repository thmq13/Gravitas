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

    using UIPanels = std::vector<std::unique_ptr<UIPanel>>;

    core::EventBus&                             m_bus;
    std::unordered_map<core::AppState, UIPanels> m_panelTable{};
  };
} // namespace gravitas::ui 