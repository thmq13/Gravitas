#pragma once

#include "UI/UIPanel.hpp"
#include "Core/AppState.hpp"

namespace gravitas::core {
  class EventBus;
}

namespace gravitas::ui
{
  class BackButtonPanel : public UIPanel {
  public:
    explicit BackButtonPanel(core::AppState targetState) noexcept;
    ~BackButtonPanel() override = default;

    void Draw(core::EventBus& bus) override;

  private:
    [[nodiscard]] bool BeginWindow() override;
    void EndWindow() override;

    core::AppState m_targetState;
  };
} // namespace gravitas::ui