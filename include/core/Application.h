#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include "Essential.h"
#include <Magnum/Platform/Sdl2Application.h>

using namespace Magnum;
namespace core {

class Application : public Platform::Application {
public:
    Application(int args = 0);

private:

    void drawEvent() override;


};

}

#endif //CORE_APPLICATION_H
