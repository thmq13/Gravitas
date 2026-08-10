#include "Core/Reflectable.hpp"

#include <cassert>

#include "Core/Logging.hpp"

namespace gravitas::core
{
  bool PropertyNode::IsBranch() const noexcept {
    return std::holds_alternative<std::vector<PropertyNode>>(payload);
  }

  const std::vector<PropertyNode>& PropertyNode::GetChildren() const {
    if (!IsBranch()) {
      assert(IsBranch() && "PropertyNode::GetChildren called on a non-branch node");
      GVT_ERROR("PropertyNode::GetChildren called on a non-branch node");
      static const std::vector<PropertyNode> empty_vector{};
      return empty_vector;
    }
    return std::get<std::vector<PropertyNode>>(payload);
  }

  PropertyNode PropertyNode::CreateBranchNode(std::string name, std::vector<PropertyNode> children) {
    return PropertyNode{
      .name    = std::move(name),
      .access  = AccessMode::ReadWrite,
      .hint    = DefaultHint{},
      .payload = std::move(children)
    };
  }
} // namespace gravitas::core