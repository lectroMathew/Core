#ifndef CORE_IDRAWABLE_H
#define CORE_IDRAWABLE_H

#include "Math.h"

namespace core {

class IDrawable {
public:

    virtual void Draw() = 0;

    // these three should not be here idk what to do with them
    virtual void SetProjectionMatrix(Matrix4& mtx) = 0;
    virtual void SetTransformMatrix(Matrix4& mtx) = 0;
    virtual void SetLight(Light& light) = 0;

};

}

#endif //CORE_IDRAWABLE_H
