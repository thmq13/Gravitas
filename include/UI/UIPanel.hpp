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
  };
} // namespace gravitas::ui 

