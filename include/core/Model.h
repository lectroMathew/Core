#ifndef CORE_MODEL_H
#define CORE_MODEL_H

#include "Essentials.h"
#include "Shader.h"
#include "Mesh.h"
#include "Math.h"

namespace core {

class Model {
public:
    Model(Mesh* mesh, Shader* shader);
    Model();
    ~Model();

    void Draw(Matrix4 &transformMtx, Camera &camera);

    /**
     * Imports and adds model to the scene
     * @param filepath - path to model file
     */
    static void Load(const string& filepath);

    Color3  color;
    Mesh*   mesh;
    Shader* shader;
};

} // namespace core

#endif //CORE_MODEL_H
