#pragma once

#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <string>
#include <string_view>
#include <typeindex>
#include <typeinfo>
#include <optional>

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

  using PropertyCopyFunc = bool(*)(void* dest, const void* src, std::size_t srcSize);

  //Trivially Copyable and POD
  template <typename Type>
  PropertyCopyFunc MakeCopyFunc() {
    static_assert(std::is_copy_assignable_v<Type>, "MakeCopyFunc: Type must be copy assignable");

    return [](void* dest, const void* src, std::size_t srcSize) -> bool {
      if (!dest || !src) {
        GVT_ERROR("MakeCopyFunc: Null pointer passed to CopyAssign");
        GVT_ASSERT(false, "MakeCopyFunc: Null pointer passed to CopyAssign");
        return false;
      }
      if (srcSize != sizeof(Type)) { 
        GVT_ERROR("MakeCopyFunc: Property size mismatch in CopyAssign between source and target type");
        GVT_ASSERT(false, "MakeCopyFunc: Property size mismatch in CopyAssign between source and target type");
        return false;
      }

      *static_cast<Type*>(dest) = *static_cast<const Type*>(src);
      return true;
    };
  }

  struct PropertyMetadata {
    std::unordered_map<ReflectType, std::string> values{};
    [[nodiscard]] bool Has(ReflectType key) const noexcept;
    [[nodiscard]] std::optional<std::string_view> Get(ReflectType key) const noexcept;
  };

  struct Reflection;
  struct PropertyNode {
    std::string name{};
    PropertyType type{};
    std::size_t offset{};
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
