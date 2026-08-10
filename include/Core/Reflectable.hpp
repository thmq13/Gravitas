#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <variant>
#include <functional>
#include <vector>
#include <numeric>
#include <utility>

#include "Utils/Math.hpp"

namespace gravitas::core
{
  // ==============================================================================
  // Access
  // ==============================================================================
  enum class AccessMode : std::uint8_t { ReadWrite = 0, ReadOnly };
   
  // ==============================================================================
  // Hint
  // ==============================================================================
  struct DefaultHint {};

  template <typename ValueType>
  struct RangeHint {
    ValueType min{ std::numeric_limits<ValueType>::lowest() };
    ValueType max{ std::numeric_limits<ValueType>::max() };
  };

  struct SelectorHint {
    std::vector<std::string> options{};
    std::string selectedOption{};
  };

  using PropertyHint = std::variant<
    DefaultHint, 
    RangeHint<int>, 
    RangeHint<std::size_t>,
    RangeHint<std::uint8_t>, 
    RangeHint<std::uint16_t>, 
    RangeHint<std::uint32_t>, 
    RangeHint<std::uint64_t>,
    RangeHint<float>, 
    RangeHint<double>,
    SelectorHint
  >;

  // ==============================================================================
  // Payload
  // ==============================================================================
  struct PropertyNode;
  using NodePayload = std::variant<
    std::monostate,
    int, 
    std::size_t, 
    std::uint8_t, 
    std::uint16_t,
    std::uint32_t,
    std::uint64_t,
    float,
    double,
    bool,
    char, 
    std::string,
    std::vector<PropertyNode>
  >;

  // ==============================================================================
  // Node
  // ==============================================================================
  struct PropertyNode {
    std::uint32_t id{};
    std::string path{};
    std::string name{};
    AccessMode access{ AccessMode::ReadWrite };
    PropertyHint hint{ DefaultHint{} };
    NodePayload payload{ std::monostate{} };

    [[nodiscard]] bool IsBranch() const noexcept;
    [[nodiscard]] const std::vector<PropertyNode>& GetChildren() const;

    [[nodiscard]] static PropertyNode CreateBranchNode(
      std::string path,
      std::string name, 
      std::vector<PropertyNode> children
    );

    template <typename ValueType>
    [[nodiscard]] static PropertyNode CreateLeafNode(
      std::string path,
      std::string name, 
      AccessMode access,
      PropertyHint hint,
      ValueType value
    ) {
      return PropertyNode{
        .id      = utils::HashPath(path),
        .path    = std::move(path),
        .name    = std::move(name),
        .access  = access,
        .hint    = std::move(hint),
        .payload = std::move(value)
      };
    }
  };

  // ==============================================================================
  // Reflectable
  // ==============================================================================
  struct EvtPropertyChange;

  class Reflectable {
  public:
    virtual ~Reflectable() = default;
    [[nodiscard]] virtual PropertyNode GetTree() const = 0;
    virtual void OnPropertyChange(const EvtPropertyChange& event) = 0;
  };
} // namespace gravitas::core