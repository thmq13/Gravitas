#include "UI/UIManager.hpp"

#include "Core/Logging.hpp"
#include "UI/UIPanel.hpp"
#include "UI/UIPanels/TitleScreenPanel.hpp"

namespace gravitas::ui 
{
  UIManager::UIManager(core::EventBus& bus) : m_bus(bus) {
    RegisterPanels();
    GVT_INFO("UIManager::UIManager succeeded");
  }

  UIManager::~UIManager() {
    GVT_INFO("UIManager::~UIManager freeing resources");
  }

  void UIManager::Draw(core::AppState state) {
    if (auto it{ m_panelTable.find(state) }; it != m_panelTable.end()) {
      for (const auto& panel : it->second) {
        if (!panel) {
          return;
        }
        panel->Draw(m_bus);
      }
    }
    core::Logger::GetInstance().Draw();
  }

  void UIManager::HandleEvent(const core::Event& event) {
    event;
  }

  void UIManager::RegisterPanels() {
    m_panelTable[core::AppState::TitleScreen].push_back(std::make_unique<TitleScreenPanel>());
  }
} // namespace gravitas::ui 