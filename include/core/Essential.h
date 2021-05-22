/**
 * This header contains forward-declarations for core and bgfx classes
 */
#ifndef CORE_ESSENTIAL_H
#define CORE_ESSENTIAL_H

//      Core defines - - - -
#ifndef SDL_MAIN_HANDLED
#   define SDL_MAIN_HANDLED
#endif

#ifndef NULL
#   define NULL 0
#endif

#ifndef __FILENAME__
#   ifdef _WIN32
#       define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)
#   endif
#   ifdef __unix__
#       define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)
#   endif
#endif
//      Core defines - - - -

#include <string>
#include <cstring>
#include <vector>
#include <cstdint>
#include <cassert>


namespace core {

class WindowModule;

class Color;
class Component;
class Transform;
class Renderer;
class Core;
class CoreConfig;
class Degree;
class Entity;
class Logger;
class Log;
class Math;
class IModuleContainer;
class Object;
class Primitive;
class Radian;
class WindowRenderer;
class Root;
class Window;
class WindowBuilder;

// Interfaces classes
class IModule;


class WindowModule;
class EngineModule;
class InputModule;
class RenderModule;

struct StatusDesc;
struct RgbaColor;
struct AbgrColor;

template<int cols, int rows, typename T> class Matrix;
template<int dims, typename T> class Vector;
template<typename T> class Pool;
template<typename T> class Node;
template<typename T> class Singleton;
template<typename T> class Instantiable;
template<typename T> class TypeMap;
template <class T, class D> class ModuleContainer;

typedef std::string string;
typedef uint32_t rgba;
typedef uint32_t abgr;

typedef struct Cube Cube;
typedef struct Rect Rect;
typedef struct ColorVertex ColorVertex;

typedef Matrix<3, 3, float> Matrix3;
typedef Matrix<3, 3, int> Matrix3i;
typedef Matrix<4, 4, float> Matrix4;
typedef Matrix<4, 4, int> Matrix4i;

typedef Vector<2, float> Vector2;
typedef Vector<2, int> Vector2i;
typedef Vector<3, float> Vector3;
typedef Vector<3, int> Vector3i;
typedef Vector<4, float> Vector4;


} // namespace core


namespace bgfx {

class PlatformData;

} // namespace bgfx


/// Defined by CORE_HEAVY_ESSENTIALS option in CMakeLists.txt
#ifdef CORE_HEAVY_ESSENTIALS
#include "Matrix.h"
#include "Vector.h"
#endif


#endif //CORE_ESSENTIAL_H
