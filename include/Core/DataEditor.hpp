#pragma once

#include "core/EventBus.hpp"

namespace gravitas::core
{
  class DataEditor : public EventBusSubscriber<DataEditor> {
  public:
    DataEditor(EventBus& bus);
    void HandleEvent(const Event& event);
  };
} // namespace gravitas::core