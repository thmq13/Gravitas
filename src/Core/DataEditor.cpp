#include "Core/DataEditor.hpp"

#include <variant>
#include <type_traits>

#include "Reflect/Reflectable.hpp"

namespace gravitas::core
{
  DataEditor::DataEditor(EventBus& bus) {
    SubscribeToEvents<EvtPropertyChange>(bus);
  }

  void DataEditor::HandleEvent(const Event& event) {
    std::visit([this](const auto& evt) {
      using EventType = std::decay_t<decltype(evt)>;

      if constexpr (std::same_as<EventType, EvtPropertyChange>) {
        if (auto target{ evt.target.lock() }) {
          target->ApplyPropertyChange(
            evt.propertyOffset,
            evt.propertyNameId,
            evt.typeId,
            evt.newData.data(),
            evt.newData.size()
          );
        } else {
          GVT_DEBUG("DataEditor::HandleEvent tried to apply change to a deleted target object");
        }
      }
    }, event);
  }
} // namespace gravitas::core