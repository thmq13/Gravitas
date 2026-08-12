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
          const bool success{target->ApplyPropertyChange(
            evt.propertyOffset,
            evt.propertyNameId,
            evt.typeId,
            evt.GetData(),
            evt.dataSize
          )};
          if (!success) {
            GVT_ERROR("DataEditor::HandleEvent failed to apply property change");
            GVT_ASSERT(false, "DataEditor::HandleEvent failed to apply property change");
          }
        } else {
          GVT_DEBUG("DataEditor::HandleEvent tried to apply change to a deleted target object");
        }
      }
    }, event);
  }
} // namespace gravitas::core
