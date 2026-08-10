#include "UI/UIPanels/BackButtonPanel.hpp"

#include <imgui.h>

#include "Core/EventBus.hpp"

namespace gravitas::ui
{
  namespace 
  {
    constexpr const char* kBackText{ "< BACK" }; 
  } // namespace

  BackButtonPanel::BackButtonPanel(core::AppState targetState) noexcept 
    : m_targetState(targetState) {}

  void BackButtonPanel::Draw(core::EventBus& bus) {
    if (!BeginWindow()) {
      EndWindow();
    }

    ImGui::SetWindowFontScale(2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    ImVec2 button_size(ImGui::CalcTextSize(kBackText));
    button_size.x *= 1.5f;
    button_size.y *= 1.5f;

    if (ImGui::Button(kBackText, button_size)) {
      bus.Publish(core::EvtRequestStateChange{ m_targetState });
    }

    ImGui::PopStyleVar();
    ImGui::SetWindowFontScale(1.0f);  

    EndWindow();
  }

  bool BackButtonPanel::BeginWindow() {
    ImGuiViewport* viewport{ ImGui::GetMainViewport() };
    ImGui::SetNextWindowPos(ImVec2{ viewport->WorkPos.x + 5.0f, 
      viewport->WorkPos.y + viewport->WorkSize.y * 0.9f });


    ImGuiWindowFlags flags{ ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoBackground | 
                            ImGuiWindowFlags_AlwaysAutoResize };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.5f, 0.5f, 0.5f, 0.5f});

    return ImGui::Begin("BackButton", nullptr, flags);
  }
  
  void BackButtonPanel::EndWindow() {
    ImGui::End();
    ImGui::PopStyleColor(2); 
  }
} // namespace gravitas::ui