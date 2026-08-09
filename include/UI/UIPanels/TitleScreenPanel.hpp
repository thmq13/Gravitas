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
    bool BeginWindow();
    void EndWindow();
  };
} // namespace gravitas::ui