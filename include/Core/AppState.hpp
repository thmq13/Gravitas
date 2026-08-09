#pragma once

#include <cstdint>

namespace gravitas::core 
{
  enum class AppState : std::uint8_t {
    Uninitialized = 0,

    TitleScreen,
    ModeSelection,

    RealTimeConfig,
    RealTimeRunning,

    PrecomputeConfig,
    PrecomputeRunning,

    PlaybackConfig,
    PlaybackRunning,

    TearDown // Changing the TearDown will not shutdown the application,
             // this is merely a marker state. Use EvtRequestShutdown.
  };
}