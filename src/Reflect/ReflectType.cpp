#include "Reflect/ReflectType.hpp"

#include <typeindex>

#include "Core/Logging.hpp"

namespace gravitas::reflect
{
  bool PropertyMetadata::Has(ReflectType key) const noexcept {
    return values.contains(key);
  }

  std::optional<std::string_view> PropertyMetadata::Get(ReflectType key) const noexcept{
    auto it{ values.find(key) };
    if (it == values.end()) {
      return std::nullopt;
    }
    return it->second;
  }

  const PropertyNode* Reflection::GetProperty(std::size_t propertyOffset) const noexcept {
    for (const auto& property : properties) {
      if (property.offset == propertyOffset) {
        return &property;
      }
    }
    return nullptr;
  }
} // namespace gravitas::reflect
