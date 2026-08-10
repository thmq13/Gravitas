#pragma once

#include "UI/UIPanel.hpp"

namespace gravitas::core {
  class EventBus;
}

namespace gravitas::ui
{
  class TitleScreenPanel : public UIPanel {
  public:
    TitleScreenPanel() noexcept = default;
    ~TitleScreenPanel() override = default;

    void Draw(core::EventBus& bus) override;

  private:
    [[nodiscard]] bool BeginWindow() override;
    void EndWindow() override;
  };
} // namespace gravitas::ui