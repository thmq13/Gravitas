#include "Reflect/ReflectType.hpp"

#include <typeindex>

#include "Core/Logging.hpp"

namespace gravitas::reflect
{
  bool PropertyMetadata::Has(ReflectType key) const noexcept {
    return values.contains(key);
  }

  std::string_view PropertyMetadata::Get(ReflectType key) const noexcept{
    auto it{ values.find(key) };
    if (it == values.end()) {
      GVT_ERROR("PropertyMetadata::Get called with a key not available in this object metadata");
      return "Key not found";
    }
    return it->second;
  }

  const PropertyNode* Reflection::GetProperty(std::size_t propertyOffset) const noexcept {
    for (const auto& property : properties) {
      if (property.memoryOffset == propertyOffset) {
        return &property;
      }
    }
    GVT_ERROR("Reflection::GetProperty called with an offset not available in {}", name);
    return nullptr;
  }
} // namespace gravitas::reflect