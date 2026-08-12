#include "Reflect/Reflectable.hpp"

#include <cassert>

#include "Reflect/ReflectType.hpp"
#include "Core/Logging.hpp"

namespace gravitas::reflect
{
  bool Reflectable::ApplyPropertyChange(
    std::size_t propertyOffset, 
    std::uint32_t propertyNameId,
    std::type_index incomingTypeId,
    const void* newData, 
    std::size_t dataSize
  ) 
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    const Reflection& reflection{ GetReflection() };

    // Bound check
    if (propertyOffset + dataSize > reflection.sizeInBytes) {
      GVT_ERROR("Reflectable::ApplyPropertyChange memory mutation out of bound for class {}", reflection.name);
      return false;
    }

    // Offset check
    const PropertyNode* property_node{ reflection.GetProperty(propertyOffset) };
    if (!property_node) {
      GVT_ERROR("Reflectable::ApplyPropertyChange property offset {} not found for class {}",
        propertyOffset, reflection.name);
      return false;
    }

    // Typeid check
    if (property_node->typeId != incomingTypeId) {
      GVT_ERROR("Reflectable::ApplyPropertyChange incoming type id mismatch for property {} in class {}",
        property_node->name, reflection.name);
      return false;
    }

    //  Copy assign
    void* dest_memory{ reinterpret_cast<char*>(this) + propertyOffset };
    if(!property_node->CopyAssign(dest_memory, newData, dataSize)) {
      GVT_ERROR("Reflectable::ApplyPropertyChange unable to copy assign property {} in class {}",
        property_node->name, reflection.name);
      return false;
    }

    ++m_version;
    PostEditChange(propertyNameId);
    return true;
  }

  [[nodiscard]] std::uint32_t Reflectable::GetVersion() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_version;
  }

} // namespace gravitas::reflect