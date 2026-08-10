#pragma once

#include <cstdlib>

#include "Core/App.hpp"
#include "Core/Logging.hpp"

namespace gravitas::entry {
  [[nodiscard]] core::App CreateApp();
}

int main() {
  auto app{ gravitas::entry::CreateApp() };

  if (auto result{ app.Initialize() }; !result) {
    GVT_FATAL("main could not initialize app: {}", result.error());
  }

  if (auto result{ app.Loop() }; !result) {
    GVT_FATAL("main failed on executing app loop: {}", result.error());
  }

  return EXIT_SUCCESS;
}