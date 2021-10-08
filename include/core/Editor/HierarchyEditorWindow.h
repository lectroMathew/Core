/*
 * This is a stand-alone header file. No SceneGraphEditorWinodw.cpp is presented.
 */
#ifndef CORE_HIERARCHYREDITORWINDOW_H
#define CORE_HIERARCHYREDITORWINDOW_H

#include "core/Essentials.h"
#include "EditorWindow.h"

namespace core {

class HierarchyEditorWindow : public EditorWindow {
public:
    HierarchyEditorWindow(ImGuiID& dockID) : EditorWindow(dockID, "Hierarchy") {

    }

    void DrawWindowContent() override {
        ImGui::Text("Wow Hierarchy!");
        // TODO: Implement Hierarchy window
    }

};

}

#endif //CORE_HIERARCHYREDITORWINDOW_H
