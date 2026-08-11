#include "Reflect/Reflectable.hpp"

#include <cassert>

#include "Reflect/ReflectType.hpp"
#include "Core/Logging.hpp"

namespace gravitas::reflect
{
  void Reflectable::ApplyPropertyChange(
    std::size_t propertyOffset, 
    std::uint32_t propertyNameId,
    std::type_index incomingTypeId,
    const void* newData, 
    std::size_t dataSize
  ) 
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    const Reflection& reflection{ GetReflection() };

    if (propertyOffset + dataSize > reflection.sizeInBytes) {
      GVT_ERROR("Reflectable::ApplyPropertyChange memory mutation out of bound for class {}", reflection.name);
      assert(false && "Reflectable::ApplyPropertyChange memory mutation out of bound");
      return;
    }

    const PropertyNode* property_node{ reflection.GetProperty(propertyOffset) };
    if (!property_node) {
      GVT_ERROR("Reflectable::ApplyPropertyChange property offset {} not found for class {}",
        propertyOffset, reflection.name);
      assert(false && "Reflectable::ApplyPropertyChange property offset not found");
      return;
    }

    if (property_node->typeId != incomingTypeId) {
      GVT_ERROR("Reflectable::ApplyPropertyChange incoming type id mismatch for property {} in class {}",
        property_node->name, reflection.name);
      assert(false && "Reflectable::ApplyPropertyChange type id mismatch");
      return;
    }

    void* dest_memory{ reinterpret_cast<char*>(this) + propertyOffset };
    property_node->CopyAssign(dest_memory, newData, dataSize);

    ++m_version;
    PostEditChange(propertyNameId);
  }

  [[nodiscard]] std::uint32_t Reflectable::GetVersion() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_version;
  }

} // namespace gravitas::reflect