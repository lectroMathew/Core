#include <core/Root.h>
#include <core/WindowPool.h>
#include <core/Logger.h>
#include <core/Renderer.h>
#include <core/Entity.h>

#include <SDL.h>
#include <memory>


namespace core {


Root::Root() {
    if ( !SDL_Init(SDL_INIT_EVERYTHING) ) {
        Logger::LogInfo("SDL initialized successfully", INTERNAL);
    } else {
        Logger::LogError("SDL initialization failed!", INTERNAL);
    }

    // Note: rootEntity is not the same as singleton core::Root object!
    // It is only used as Entity hierarchy root.
    Entity* rootEntity = new Entity();
    Entity::SetRoot(rootEntity);

    logger = std::make_unique<Logger>();
    renderer = std::make_unique<Renderer>();
    windowPool = std::make_unique<WindowPool>();

    Logger::LogInfo("- Core initialization complete -", INTERNAL);
}
Root::~Root() {
    windowPool->DestroyAll();
    renderer->DestroyAll();
    renderer->Destroy();

    SDL_Quit();

    Logger::LogInfo("- Core shutdown complete -", INTERNAL);
}

}