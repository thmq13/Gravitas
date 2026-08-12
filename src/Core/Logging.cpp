#include "Core/Logging.hpp"

#include <iostream>
#include <imgui.h>

namespace gravitas::core
{
  namespace
  {
    [[nodiscard]] constexpr std::string_view ToString(LogLevel level) noexcept {
      switch (level) {
        case LogLevel::Debug: return "[DEBUG] ";
        case LogLevel::Trace: return "[TRACE] ";
        case LogLevel::Info:  return "[INFO] ";
        case LogLevel::Warn:  return "[WARN] ";
        case LogLevel::Error: return "[ERROR] ";
        case LogLevel::Fatal: return "[FATAL] ";
        default:              return "[UNKNOWN] ";
      }
    }

    [[nodiscard]] constexpr ImVec4 ToColor(LogLevel level) noexcept {
      switch (level) {
        case LogLevel::Debug: return ImVec4{0.2f, 0.6f, 1.0f, 1.0f};
        case LogLevel::Trace: return ImVec4{0.5f, 0.5f, 0.5f, 1.0f};
        case LogLevel::Info:  return ImVec4{0.2f, 0.8f, 0.2f, 1.0f};
        case LogLevel::Warn:  return ImVec4{0.9f, 0.7f, 0.0f, 1.0f};
        case LogLevel::Error: return ImVec4{0.9f, 0.2f, 0.2f, 1.0f};
        case LogLevel::Fatal: return ImVec4{1.0f, 0.1f, 1.0f, 1.0f};
        default:              return ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
      }
    }

    constexpr const char* kLogFilePath{ "nbody.log" };

    [[nodiscard]] bool BeginLoggerWindow() {
      ImGuiViewport* viewport{ ImGui::GetMainViewport() };
      const float window_width{ viewport->Size.x * 0.35f };
      const float window_height{ viewport->Size.y * 0.35f };

      ImGui::SetNextWindowSize(ImVec2{window_width, window_height}, ImGuiCond_FirstUseEver);

      return ImGui::Begin("Logger", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNavFocus);
    }

    void EndLoggerWindow() {
      ImGui::SetWindowFontScale(1.0f);
      ImGui::End();
    }
  } // namespace

  Logger& Logger::GetInstance() {
    static Logger instance{};
    return instance;
  }

  void Logger::Submit(LogLevel level, std::string message) {
    message.insert(0, ToString(level));

    if (level >= LogLevel::Error) {
      std::cerr << message << '\n';
    } else {
      std::cout << message << '\n';
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_logFile.is_open()) {
      m_logFile << message << '\n';
      m_logFile.flush();
    }

    m_messages.push_back(LogMessage{level, std::move(message)});

    if (m_messages.size() > kMaxMessageCount) {
      m_messages.pop_front();
    }
  }

  void Logger::Draw() {
    if (!BeginLoggerWindow()) {
      EndLoggerWindow();
      return;
    }

    if (ImGui::Button("Clear")) {
      Clear();
    }

    ImGui::SameLine();
    bool copy_to_clipboard{ ImGui::Button("Copy") };

    ImGui::SameLine();
    static ImGuiTextFilter filter{};
    filter.Draw("Filter", -100.0f);
    ImGui::Separator();

    const float footer_height{ ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() };
    if (ImGui::BeginChild("Log Console Scrolling Region", ImVec2{0, -footer_height}, false, ImGuiWindowFlags_HorizontalScrollbar)) {
      if (copy_to_clipboard) {
        ImGui::LogToClipboard();
      }
          
      std::lock_guard<std::mutex> lock(m_mutex);

      ImGui::SetWindowFontScale(1.25f);
      for (const auto& msg : m_messages) {
        if (!filter.PassFilter(msg.text.c_str())) {
          continue;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, ToColor(msg.level));
        ImGui::TextUnformatted(msg.text.c_str());
        ImGui::PopStyleColor();
      }
      ImGui::SetWindowFontScale(1.0f);

      if (copy_to_clipboard) { 
        ImGui::LogFinish();
      }

      if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
      }
    }
    ImGui::EndChild();
    ImGui::Separator();
    ImGui::Checkbox("Auto-scroll", &m_autoScroll);

    EndLoggerWindow();
  }

  void Logger::Clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_messages.clear();
  }

  Logger::Logger() {
    OpenLogFile();
  }

  void Logger::OpenLogFile() {
    m_logFile.open(kLogFilePath, std::ios::out | std::ios::trunc);
    if (!m_logFile.is_open()) {
      std::string error_message{ std::format("Logger::OpenLogFile cannot open '{}'", kLogFilePath) };
      Submit(LogLevel::Error, std::move(error_message));
      return;
    } else {
      m_logFile << "--- LOG SESSION STARTED ---\n";
      m_logFile.flush();
    }
  }
} // namespace gravitas::core
