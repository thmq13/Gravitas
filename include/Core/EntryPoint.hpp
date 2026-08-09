#pragma once

#include <cstdlib>

#include "Core/App.hpp"
#include "Core/Logging.hpp"

namespace gravitas::entry {
  core::App CreateApp();
}

int main() {
  auto app{ gravitas::entry::CreateApp() };

  if (auto initResult{ app.Initialize() }; !initResult) {
    GVT_FATAL("main could not initialize app: {}", initResult.error());
  }

  if (auto loopResult{ app.Loop() }; !loopResult) {
    GVT_FATAL("main failed on executing app loop: {}", loopResult.error());
  }

  return EXIT_SUCCESS;
}