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
#ifndef CORE_EVENT_H
#define CORE_EVENT_H

#include "core/Essentials.h"

#include <vector>
#include <utility>
#include <functional>
#include <unordered_set>

namespace core {

template<typename T, typename ...Args>
class Event {
public:
    using callbackList = std::vector<std::function<void(T&, Args...)>>;

    void Subscribe(std::function<void(T&, Args...)> callback);
    void Trigger(T& obj, Args... args);
    void Trigger(T&& obj, Args... args);

protected:

    callbackList callbacks;
    uint32_t id { ++eventCounter };

private:
    static std::atomic<uint32_t> eventCounter;
};

} // namespace core

#include "Event.tpp"

#endif //CORE_EVENT_H
