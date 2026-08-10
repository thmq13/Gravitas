#include "UI/UIPanels/ModeSelectionPanel.hpp"

#include <imgui.h>

#include "Core/EventBus.hpp"

namespace gravitas::ui
{
  namespace 
  {
    constexpr const char* kSimulationModesText{ "SIMULATION MODES" }; 
    constexpr const char* kRealTimeText{ "REAL-TIME" }; 
    constexpr const char* kPrecomputeText{ "PRECOMPUTE" }; 
    constexpr const char* kPlaybackText{ "PLAYBACK" };
  } // namespace

  void ModeSelectionPanel::Draw(core::EventBus& bus) {
    if (!BeginWindow()) {
      EndWindow();
    }

    const ImVec2 window_size{ ImGui::GetWindowSize() };

    ImGui::SetWindowFontScale(4.0f);
    const ImVec2 text_size{ ImGui::CalcTextSize(kSimulationModesText) };
    ImGui::SetCursorPos(ImVec2{(window_size.x - text_size.x) * 0.5f, window_size.y * 0.30f});
    ImGui::TextUnformatted(kSimulationModesText);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetWindowFontScale(2.0f);
    ImVec2 button_size(ImGui::CalcTextSize(kPrecomputeText));
    button_size.x *= 1.5f;
    button_size.y *= 1.5f;
    float  buttonPosX{ (window_size.x - button_size.x) * 0.5f };
    float  real_time_button_pos_Y{ window_size.y * 0.55f };
    float  precompute_button_pos_Y{ real_time_button_pos_Y + button_size.y + window_size.y * 0.05f };
    float  playback_button_pos_Y{ precompute_button_pos_Y + button_size.y + window_size.y * 0.05f };

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);

    ImGui::SetCursorPos(ImVec2{buttonPosX, real_time_button_pos_Y});
    if (ImGui::Button(kRealTimeText, button_size)) {
      bus.Publish(core::EvtRequestStateChange{ core::AppState::RealTimeConfig });
    }

    ImGui::SetCursorPos(ImVec2{buttonPosX, precompute_button_pos_Y});
    if (ImGui::Button(kPrecomputeText, button_size)) {
      bus.Publish(core::EvtRequestStateChange{ core::AppState::PrecomputeConfig });
    }

    ImGui::SetCursorPos(ImVec2{buttonPosX, playback_button_pos_Y});
    if (ImGui::Button(kPlaybackText, button_size)) {
      bus.Publish(core::EvtRequestStateChange{ core::AppState::PlaybackConfig });
    }

    ImGui::PopStyleVar();
    ImGui::SetWindowFontScale(1.0f);  

    EndWindow();
  }

  bool ModeSelectionPanel::BeginWindow() {
    ImGuiViewport* viewport{ ImGui::GetMainViewport() };
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags flags{ ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize | 
                            ImGuiWindowFlags_NoBringToFrontOnFocus };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.5f, 0.5f, 0.5f, 0.5f});

    return ImGui::Begin("ModeSelection", nullptr, flags);
  }
  
  void ModeSelectionPanel::EndWindow() {
    ImGui::End();
    ImGui::PopStyleColor(2); 
  }
} // namespace gravitas::ui