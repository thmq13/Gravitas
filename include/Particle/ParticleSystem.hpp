#pragma once

#include <vector>
#include <cassert>
#include <cstddef>

#include <raylib.h>

namespace gravitas::particle
{
  struct ParticleSystem {
    std::vector<double> mass{};
    std::vector<double> positionX{};
    std::vector<double> positionY{};
    std::vector<double> positionZ{};
    std::vector<double> velocityX{};
    std::vector<double> velocityY{};
    std::vector<double> velocityZ{};
    std::vector<double> accelerationX{};
    std::vector<double> accelerationY{};
    std::vector<double> accelerationZ{};
    std::vector<float>  radius{};
    std::vector<Color>  color{};

    [[nodiscard]] std::size_t GetSize() const noexcept {
      assert(mass.size() == positionX.size() && mass.size() == velocityY.size()
        && "ParticleSystem::GetSize found size mismatch between containers");
      return mass.size();
    }

    void AddSize(std::size_t additionalSize) {
      Resize(GetSize() + additionalSize);
    }

    void Resize(std::size_t newSize) {
      mass.resize(newSize);
      positionX.resize(newSize);
      positionY.resize(newSize);
      positionZ.resize(newSize);
      velocityX.resize(newSize);
      velocityY.resize(newSize);
      velocityZ.resize(newSize);
      accelerationX.resize(newSize);
      accelerationY.resize(newSize);
      accelerationZ.resize(newSize);
      radius.resize(newSize);
      color.resize(newSize);
    }

    void Reserve(std::size_t capacity) {
      mass.reserve(capacity);
      positionX.reserve(capacity);
      positionY.reserve(capacity);
      positionZ.reserve(capacity);
      velocityX.reserve(capacity);
      velocityY.reserve(capacity);
      velocityZ.reserve(capacity);
      accelerationX.reserve(capacity);
      accelerationY.reserve(capacity);
      accelerationZ.reserve(capacity);
      radius.reserve(capacity);
      color.reserve(capacity);
    }
  };
} // namespace gravitas::particle
