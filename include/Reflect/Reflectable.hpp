#pragma once

#include <cstdint>
#include <cstddef>
#include <mutex>
#include <typeindex>

namespace gravitas::reflect
{
  struct Reflection;

  class Reflectable {
  public:
    virtual ~Reflectable() = default;

    [[nodiscard]] virtual const Reflection& GetReflection() const noexcept = 0;
    virtual void PostEditChange(std::uint32_t /*propertyNameId*/) {}

    [[nodiscard]] bool ApplyPropertyChange(
      std::size_t propertyOffset, 
      std::uint32_t propertyNameId,
      std::type_index incomingTypeId,
      const void* newData, 
      std::size_t dataSize
    );

    [[nodiscard]] std::uint32_t GetVersion() const;

  private:
    std::uint32_t m_version{ 0 };
    mutable std::mutex m_mutex{};
  };
} // namespace gravitas::reflect