/*
 * This is a stand-alone header file. No SceneGraphEditorWinodw.cpp is presented.
 */
#ifndef CORE_INSPECTOREDITOR_H
#define CORE_INSPECTOREDITOR_H

#include "core/Essentials.h"
#include "EditorWindow.h"

namespace core {

class InspectorEditor : public EditorWindow {
public:
    InspectorEditor(ImGuiID& parent, ImGuiDir direction, float size)
    : EditorWindow(parent, "Inspector", direction, size)
    {

    }

    void DrawWindowContent() override {
        ImGui::Text("No way!! Inspector it is!");
        // TODO: Implement Inspector window
    }

};

}

#endif //CORE_INSPECTOREDITOR_H
