#include "UI/UIPanels/TitleScreenPanel.hpp"

#include <imgui.h>

#include "Core/EventBus.hpp"
#include "Core/AppState.hpp"

namespace gravitas::ui
{
  namespace 
  {
    constexpr const char* kTitleText{ "G R A V I T A S" }; 
    constexpr const char* kStartText{ "START" }; 
    constexpr const char* kExitText{ "EXIT" }; 
  } // namespace

  void TitleScreenPanel::Draw(core::EventBus& bus) {
    if (!BeginWindow()) {
      EndWindow();
      return;
    }

    ImVec2 window_size{ ImGui::GetWindowSize() };
    
    ImGui::SetWindowFontScale(4.0f);
    ImVec2 text_size{ ImGui::CalcTextSize(kTitleText) };
    ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, window_size.y * 0.30f));
    ImGui::TextUnformatted(kTitleText);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetWindowFontScale(2.0f);
    ImVec2 button_size(ImGui::CalcTextSize(kStartText));
    button_size.x *= 1.5f;
    button_size.y *= 1.5f;
    float  buttonPosX{ (window_size.x - button_size.x) * 0.5f };
    float  start_button_pos_Y{ window_size.y * 0.55f };
    float  exit_button_pos_Y{ start_button_pos_Y + button_size.y + window_size.y * 0.05f };

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    ImGui::SetCursorPos(ImVec2(buttonPosX, start_button_pos_Y));
    if (ImGui::Button("START", button_size)) {
      bus.Publish(core::EvtRequestStateChange{core::AppState::ModeSelection});
    }
    
    ImGui::SetCursorPos(ImVec2(buttonPosX, exit_button_pos_Y));
    if (ImGui::Button("EXIT", button_size)) {
      bus.Publish(core::EvtRequestShutdown{});
    }

    ImGui::PopStyleVar();
    ImGui::SetWindowFontScale(1.0f);

    EndWindow();
  }

  bool TitleScreenPanel::BeginWindow() {
    ImGuiViewport* viewport{ ImGui::GetMainViewport() };
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags flags{ ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize | 
                            ImGuiWindowFlags_NoBringToFrontOnFocus };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.5f, 0.5f, 0.5f, 0.5f});

    return ImGui::Begin("TitleScreen", nullptr, flags);
  }

  void TitleScreenPanel::EndWindow() {
    ImGui::End();
    ImGui::PopStyleColor(2); 
  }
} // namespace gravitas::ui