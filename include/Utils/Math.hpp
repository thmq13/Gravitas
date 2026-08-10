#pragma once

#include <cstdint>
#include <string_view>

namespace gravitas::utils
{
  [[nodiscard]] constexpr std::uint32_t HashPath(std::string_view str) noexcept {
    std::uint32_t hash{ 2166136261U };
    for (char c : str) { 
      hash ^= static_cast<std::uint32_t>(c); 
      hash *= 16777619u; 
    }
    return hash;
  }
} // namespace gravitas::utils