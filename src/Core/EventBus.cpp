#include "Core/EventBus.hpp"

#include <exception>

namespace gravitas::core
{
  void EventBus::Publish(Event event) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(event));
  }

  void EventBus::Dispatch() {
    std::queue<Event> local_queue{};
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::swap(local_queue, m_queue);
    }

    while (!local_queue.empty()) {
      Event event{ std::move(local_queue.front()) };
      local_queue.pop();

      std::type_index event_id{ 
        std::visit([](const auto& evt) -> std::type_index {
          return typeid(evt);
        }, event)
      };

      std::vector<EventCallback> callbacks_to_invoke;
      {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it{ m_callbacks.find(event_id) };
        if (it != m_callbacks.end()) {
          callbacks_to_invoke = it->second;
        }
      }

      if (callbacks_to_invoke.empty()) {
        GVT_WARN("EventBus::Dispatch found an event type with no listeners: {}.", event_id.name());
        continue;
      }

      for (const auto& callback : callbacks_to_invoke) {
        try {
          callback(event);
        } catch (const std::exception& e) {
          GVT_ERROR("EventBus::Dispatch caught an exception during callback for event type '{}': {}.",
            event_id.name(), e.what());
        } catch (...) {
          GVT_ERROR("EventBus::Dispatch caught an unknown exception during callback for event type '{}'",
            event_id.name());
        }
      }
    }
  }
} // namespace gravitas::core
