//
// Created by Sebastian on 21.03.2024.
//

#ifndef OPENGL_QUEUEEDITORWINDOW_H
#define OPENGL_QUEUEEDITORWINDOW_H
#include <utility>
#include <vector>
#include <unordered_map>
#include "imgui.h"
#include "RenderQueue.h"

class QueueEditorWindow {
private:
    std::vector<RenderQueue*> m_renderQueues;
    RenderEntity* m_currentSelectedEntity{};
    std::unordered_map<std::string, size_t> m_selectionStateEntity;
    std::string m_currentRenderQueueName;

public:
    static constexpr int WINDOW_WIDTH = 400;

    QueueEditorWindow(std::initializer_list<RenderQueue*> renderQueues) : m_renderQueues((renderQueues)) {
        for (size_t i = 0; i < m_renderQueues.size(); i++) {
            std::string queueName = m_renderQueues[i]->getName();

            if (i == 0) {
                m_selectionStateEntity[queueName] = 0;
                m_currentSelectedEntity = &m_renderQueues[i]->getQueue().at(i);
                m_currentRenderQueueName = queueName;
            } else {
                m_selectionStateEntity[queueName] = -1;
            }
        }
    }

    void render() {
        const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
        const float windowWidth = mainViewport->Size.x;

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, 800));
        ImGui::SetNextWindowPos(ImVec2(windowWidth - WINDOW_WIDTH, 0));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_MenuBar;

        if (ImGui::Begin("Queue editor", nullptr, windowFlags)) {
            // Menu
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Test")) {
                    ImGui::MenuItem("(Test menu)");

                    if (ImGui::MenuItem("One")) {
                        std::cout << "Test" << std::endl;
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            // Queues
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Render queues");
            ImGui::BeginChild("QueuesWindow", ImVec2(WINDOW_WIDTH, 200), ImGuiChildFlags_None);

            for (RenderQueue* queue : m_renderQueues) {
                if (ImGui::TreeNodeEx(queue->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                    std::vector<RenderEntity>& currentQueue = queue->getQueue();

                    for (size_t i = 0; i < currentQueue.size(); i++) {
                        RenderEntity* entity = &currentQueue.at(i);

                        if (ImGui::Selectable(entity->getModel().name.c_str(), m_selectionStateEntity[queue->getName()] == i)) {
                            // Reset selection state of all nodes
                            for (auto& it : m_selectionStateEntity) {
                                it.second = -1;
                            }

                            m_selectionStateEntity[queue->getName()] = i;
                            m_currentSelectedEntity = entity;
                            m_currentRenderQueueName = queue->getName();
                        }
                    }
                    ImGui::TreePop();
                }
            }

            // End child window
            ImGui::EndChild();
            handleRenderEntityProperties();
        }

        // End parent window
        ImGui::End();
    }

    void handleRenderEntityProperties() {
        ImGui::SeparatorText("Render entity properties");

        if (m_currentSelectedEntity == nullptr) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No entity selected");
            return;
        }

        const Model& model = m_currentSelectedEntity->getModel();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Queue: %s", m_currentRenderQueueName.c_str());
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Model: %s", model.name.c_str());

        // Get total vert num
        size_t count = 0;

        for (const MeshPrimitive& primitive : model.primitives) {
            count += primitive.count;
        }

        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Verts: %zu", count);
        ImGui::Separator();

        ImGui::Text("Model Matrix");
        editMat4("modelMatrix", m_currentSelectedEntity->getModelMatrix());

        ImGui::Separator();

        // Transformations
        ImGui::Text("Transformations");

        glm::vec3 newTransVec = editVec3("Translation", m_currentSelectedEntity->getTranslationVec());
        m_currentSelectedEntity->setTranslationVec(newTransVec);

        glm::vec3 newRotationAxis = editVec3("Rotation Axis", m_currentSelectedEntity->getRotationVec());
        m_currentSelectedEntity->setRotationVec(newRotationAxis);

        float rotAngle = m_currentSelectedEntity->getRotationAngle();
        ImGui::InputFloat("Rotation degree", &rotAngle, 1.0f);
        m_currentSelectedEntity->setRotationAngle(rotAngle);

        glm::vec3 newScaleVec = editVec3("Scale", m_currentSelectedEntity->getScaleVec());
        m_currentSelectedEntity->setScaleVec(newScaleVec);

        m_currentSelectedEntity->recalculateTransformsAndMatrices(true);
    }

    void editMat4(const std::string& id, glm::mat4& matrix) {
        float columnWidth = ImGui::GetContentRegionAvail().x * 0.25f - 15;

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                std::string fieldId = id + std::to_string(row * 4 + col);
                ImGui::PushID(fieldId.c_str());
                ImGui::PushItemWidth(columnWidth);

                ImGui::InputFloat(" ", &matrix[row][col]);

                ImGui::PopID();
                ImGui::PopItemWidth();
                ImGui::SameLine();
            }

            ImGui::NewLine();
        }
    }

    glm::vec3 editVec3(const std::string& id, glm::vec3 vec) {
        float vec3[3] = { vec.x, vec.y, vec.z };
        ImGui::InputFloat3(id.c_str(), vec3);
        return glm::vec3{ vec3[0], vec3[1], vec3[2] };
    }
};


#endif //OPENGL_QUEUEEDITORWINDOW_H
