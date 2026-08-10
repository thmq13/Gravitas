#pragma once

namespace gravitas::core { 
  class EventBus; 
}

namespace gravitas::ui 
{
  class UIPanel {
  public:
    virtual ~UIPanel() = default;
    virtual void Draw(core::EventBus& bus) = 0;
  protected:
    [[nodiscard]] virtual bool BeginWindow() = 0;
    virtual void EndWindow() = 0;
  };
} // namespace gravitas::ui 

