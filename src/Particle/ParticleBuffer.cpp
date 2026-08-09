#include "Particle/ParticleBuffer.hpp"

#include <utility>

namespace gravitas::particle
{
  void ParticleBuffer::CommitBackBuffer() {
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::swap(m_backBuffer, m_sharedBuffer);
    }
    m_isNewFrameReady.store(true);
  }

  void ParticleBuffer::UpdateFrontBuffer() {
    if (m_isNewFrameReady.exchange(false)) {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::swap(m_sharedBuffer, m_frontBuffer);
    }
  }

  ParticleSystem& ParticleBuffer::GetBackBuffer() noexcept { 
    return m_backBuffer; 
  }

  const ParticleSystem& ParticleBuffer::GetFrontBuffer() const noexcept {
    return m_frontBuffer;
  }
} // namespace gravitas::particle
