#pragma once

#include <atomic>
#include <mutex>

#include <Particle/ParticleSystem.hpp>

namespace gravitas::particle
{
  class ParticleBuffer {
  public:
    ParticleBuffer() noexcept = default;

    ~ParticleBuffer() = default;  

    ParticleBuffer(const ParticleBuffer&) = delete;
    ParticleBuffer& operator=(const ParticleBuffer&) = delete;
    ParticleBuffer(ParticleBuffer&&) noexcept = delete;
    ParticleBuffer& operator=(ParticleBuffer&&) noexcept = delete;

    void CommitBackBuffer();
    void UpdateFrontBuffer();
    [[nodiscard]] ParticleSystem& GetBackBuffer() noexcept;
    [[nodiscard]] const ParticleSystem& GetFrontBuffer() const noexcept;

  private:
    ParticleSystem     m_frontBuffer{};
    ParticleSystem     m_backBuffer{};
    ParticleSystem     m_sharedBuffer{};
    std::atomic<bool>  m_isNewFrameReady{ false };
    mutable std::mutex m_mutex{};
  };
} // namespace gravitas::particle
