//
// Created by Sebastian on 18.02.2024.
//

#ifndef OPENGL_CONTROLSWINDOW_H
#define OPENGL_CONTROLSWINDOW_H
#include "imgui.h"

class ControlsWindow {
public:
    void render() {
        const char* controlStrings[][2] = {
                {"E", "Toggle mouse focus"},
                {"W", "Move forward"},
                {"A", "Move left"},
                {"S", "Move backwards"},
                {"D", "Move right"},
                {"SPACE", "move up"},
                {"C", "move down"}
        };

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::BeginTable("Controls", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);

        ImVec2 cellPadding(10.0f, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cellPadding);

        ImGui::TableSetupColumn("Key");
        ImGui::TableSetupColumn("Action");

        ImGui::TableHeadersRow();

        for (const auto& row : controlStrings) {
            ImGui::TableNextRow();

            for (size_t column = 0; column < 2; column++) {
                ImGui::TableSetColumnIndex(column);
                ImGui::Text("%s", row[column]);
            }
        }

        ImGui::PopStyleVar();
        ImGui::EndTable();
        ImGui::End();
    }
};


#endif //OPENGL_CONTROLSWINDOW_H
