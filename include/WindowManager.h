#ifndef CORE_WINDOWMANAGER_H
#define CORE_WINDOWMANAGER_H

#include "Singleton.h"
#include "Window.h"

#include <vector>
#include <memory>


namespace core {

class WindowManager : public Singleton<WindowManager> {
public:

    /// Returns uint32_t id of the created window on success,
    uint32_t CreateWindow(const std::string& title, const Rect& rect);
    uint32_t CreateWindow(const std::string& title, const Vector2& pos, int width, int height);
    uint32_t CreateWindow(const std::string& title, const Vector2& pos, const Vector2& res);
    void AddWindowToPool(const Window& window);
    void AddWindowToPool(const std::shared_ptr<Window>& windowPtr);

protected:
    std::vector<std::shared_ptr<Window>> windowsPool_;

private:

};


}


#endif //CORE_WINDOWMANAGER_H
