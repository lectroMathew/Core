/*
 MIT License
 
 Copyright (c) 2021 lectroMathew <contact@lectro.moe>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#ifndef CORE_APPLICATIONMODULE_H
#define CORE_APPLICATIONMODULE_H

#include <core/Essentials.h>
#include <core/IModule.h>
#include <core/Application/Application.h>

namespace core {

class ApplicationModule : public IModule {
public:
    ApplicationModule(InputModule* inputModule);

    void CreateApplication(const string &title = "Core Application", Rect rect = { 0, 0, 1280, 720 });

    Vector2 GetWindowDpiScale(uint32_t id = 0);
    Vector2i GetWindowSize(uint32_t id = 0);
    Vector2i GetFrameBufferSize(uint32_t id = 0);

    void SwapRedraw();
    void Redraw();

protected:

    void Start() override;
    void EarlyTick() override;
    void LateTick() override;

private:

    std::vector<Application*> apps;
    InputModule* inputModule;
};

}

#endif //CORE_APPLICATIONMODULE_H
