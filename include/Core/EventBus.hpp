#pragma once

#include <cassert>
#include <concepts>
#include <functional>
#include <variant>
#include <queue>
#include <typeindex>
#include <typeinfo>
#include <mutex>
#include <utility>

#include "Core/AppState.hpp"
#include "Core/Logging.hpp"

namespace gravitas::core
{
  struct EvtRequestShutdown {
  };

  struct EvtRequestStateChange {
    AppState requestedState;
  };

  using Event = std::variant<EvtRequestShutdown, EvtRequestStateChange>;

  class EventBus {
  public:
    using EventCallback = std::function<void(const Event&)>;

    EventBus() noexcept = default;

    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus(EventBus&&) noexcept = delete;
    EventBus& operator=(EventBus&&) noexcept = delete;

    template <typename EventType>
    void Subscribe(EventCallback callback) {
      assert(callback && "EventBus::Subscribe received an empty/null callback");
      if (!callback) {
        GVT_ERROR("EventBus::Subscribe received an empty/null callback");
        return;
      }
      std::lock_guard<std::mutex> lock(m_mutex);
      m_callbacks[typeid(EventType)].push_back(std::move(callback));
    }

    void Publish(Event event);
    void Dispatch();

  private:
    std::queue<Event>                                               m_queue{};
    std::unordered_map<std::type_index, std::vector<EventCallback>> m_callbacks{};
    mutable std::mutex                                              m_mutex{};
  };

  template <typename Derived>
  class EventBusSubscriber {
  protected:
    template <typename... EventTypes>
    void SubscribeToEvents(EventBus& bus) {
      static_assert(requires(Derived& subscriber, const Event& event) {
        { subscriber.HandleEvent(event) } -> std::same_as<void>;
      }, 
        "EventBusSubscriber: Expected signature on Derived: \n"
        "void HandleEvent(const gravitas::core::Event& event)\n"
      );
      (SubscribeToEvent<EventTypes>(bus), ...);
    }

  private:
    template <typename EventType>
    void SubscribeToEvent(EventBus& bus) {
      auto* self = static_cast<Derived*>(this);
      bus.template Subscribe<EventType>([self](const Event& event) {
        self->HandleEvent(event); 
      });
    }
  };
} // namespace gravitas::core
