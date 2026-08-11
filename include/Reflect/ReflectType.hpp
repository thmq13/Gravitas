#pragma once

#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <string>
#include <string_view>
#include <memory>
#include <typeindex>

#include "Core/Logging.hpp"

namespace gravitas::reflect
{

  enum class PropertyType : std::uint8_t { 
    Bool = 0, 
    Int, 
    Float, // float and double
    String, 
    Enum, 
    Object
  }; 

  enum class ReflectType : std::uint8_t { 
    Widget = 0, 
    Number, 
    Min, 
    Max,
    Options
  };

  using PropertyCopyFunc = void(*)(void* dest, const void* src, std::size_t srcSize);

  template <typename Type>
  PropertyCopyFunc MakeCopyFunc() {
    return [](void* dest, const void* src, std::size_t srcSize) {
      if (srcSize != sizeof(Type)) { 
        GVT_ERROR("Property size mismatch in CopyAssign between source and target type");
        assert(false && "Property size mismatch in CopyAssign between source and target type");
        return;
      }
      *static_cast<Type*>(dest) = *static_cast<const Type*>(src);
    };
  }

  struct PropertyMetadata {
    std::unordered_map<ReflectType, std::string> values{};
    [[nodiscard]] bool Has(ReflectType key) const noexcept;
    [[nodiscard]] std::string_view Get(ReflectType key) const noexcept;
  };

  struct Reflection;
  struct PropertyNode {
    std::string name{};
    PropertyType type{};
    std::size_t memoryOffset{};
    std::size_t sizeInBytes{};
    std::type_index typeId{ typeid(int) };
    PropertyMetadata meta{};
    const Reflection* reflection{}; // Use only if type == Object

    PropertyCopyFunc CopyAssign{ MakeCopyFunc<int>() };
  };

  struct Reflection {
    std::string name{};
    std::size_t sizeInBytes{}; // Total memory footprint of the class
    std::vector<PropertyNode> properties{};
    [[nodiscard]] const PropertyNode* GetProperty(std::size_t propertyOffset) const noexcept;
  };
} // namespace gravitas::reflect