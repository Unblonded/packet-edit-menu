#pragma once
#include <cstdio>
#include "include/imgui/imgui.h"

class CyberpunkConsole {
public:
    enum ConsoleColor {
        COLOR_DEFAULT,
        COLOR_INFO,
        COLOR_WARN,
        COLOR_ERROR
    };

    CyberpunkConsole() {
        input_buf[0] = '\0';
        show_console = true;
        auto_scroll = true;
        filter.Clear();
    }

    // Call this every frame inside your ImGui frame
    void draw() {
        if (!show_console) return;

        ImGui::Begin("Quantum Terminal", &show_console, ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
            for (size_t i = 0; i < log_items.size(); i++) {
                if (!filter.PassFilter(log_items[i].c_str()))
                    continue;

                ImGui::PushStyleColor(ImGuiCol_Text, log_colors[i]);
                ImGui::TextUnformatted(log_items[i].c_str());
                ImGui::PopStyleColor();
            }

            if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();

        bool reclaim_focus = false;
        if (ImGui::InputText("##Input", input_buf, sizeof(input_buf),
            ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (input_buf[0]) {
                log(COLOR_INFO, "# %s", input_buf);
                on_command(input_buf);
                input_buf[0] = '\0';
            }
            reclaim_focus = true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Send")) {
            if (input_buf[0]) {
                log(COLOR_INFO, "# %s", input_buf);
                on_command(input_buf);
                input_buf[0] = '\0';
            }
            reclaim_focus = true;
        }

        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1);

        ImGui::End();
    }

    // Logs a formatted message with a color
    void log(ConsoleColor color, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);

        log_items.push_back(std::string(buf));
        switch (color) {
        case COLOR_INFO:  log_colors.push_back(ImVec4(0.18f, 0.75f, 1.00f, 1.00f)); break; // neural_blue
        case COLOR_WARN:  log_colors.push_back(ImVec4(1.00f, 0.97f, 0.20f, 1.00f)); break; // solid_yellow
        case COLOR_ERROR: log_colors.push_back(ImVec4(1.00f, 0.20f, 0.25f, 1.00f)); break; // solid_red
        default:          log_colors.push_back(ImVec4(0.99f, 0.99f, 1.00f, 1.00f)); break; // hologram_white
        }
    }

    // You can override this to handle commands
    virtual void on_command(const char* command) {
        // Default: echo the command in green
        log(COLOR_INFO, "Executed command: '%s'", command);
    }

    // Shows or hides the console window
    bool show_console = true;

private:
    char input_buf[256];
    std::vector<std::string> log_items;
    std::vector<ImVec4> log_colors;
    ImGuiTextFilter filter;
    bool auto_scroll;
};
