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
#ifndef CORE_SCENEDATA_H
#define CORE_SCENEDATA_H

#include "core/Essentials.h"

#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Trade/PhongMaterialData.h>
#include <Magnum/Trade/SceneData.h>

#include <core/Logger.h>
#include <core/Scene/Scene.h>
#include <core/Entity.h>
#include <core/Material.h>

#include <optional>

using namespace Magnum;
namespace core {

class SceneData {
public:
    
    /*
     * Creates entities with models from SceneData
     * @note Each Entity in return vector has Transform and Renderer components added
     * @param sceneData - SceneData to import from
     * @return vector<shared<Entity>>
     */
    vector<shared<Entity>> ImportEntities();
    
    /**
     * Imports models from SceneData
     * @param sceneData - SceneData to import from
     * @return vector<shared<Model>>
     */
    vector<shared<Model>> ImportModels();
    
    vector<optional<Trade::MaterialData>> materials;
    vector<unique<Trade::ObjectData3D>>   objects;
    vector<optional<GL::Texture2D>>       textures;
    vector<optional<GL::Mesh>>            meshes;
    vector<string>                        names;
    
    optional<Trade::SceneData>            children;

protected:
    
    /**
     * Add entity and its children to container
     * @param container - container to add Entities to
     * @param parent - parent of the root nodes
     * @param id - SceneData Entity id
     */
    void AddEntity(vector<shared<Entity>>& container, const shared<Entity>& parent, uint32_t id);
    
    /**
     * Add Model and its children to container
     * @param container - container to add Models to
     * @param id - SceneData Model id
     */
    void AddModel(vector<shared<Model>>& container, uint32_t id);
    
    /**
     * Add textures to the material
     * @param material - Material to add textures to
     * @param id - SceneData Material id
     */
    void SetTextures(const shared<Material>& material, uint32_t id);
    
    /**
     * Load Model from SceneData
     * @param id - id of the model
     * @return shared<Model>
     */
    shared<Model> LoadModel(uint32_t id);
    
};

}

#endif //CORE_SCENEDATA_H
