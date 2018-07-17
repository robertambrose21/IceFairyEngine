#include "sceneobject.h"

using namespace IceFairy;

_SceneObject::_SceneObject()
{ }

void _SceneObject::Draw(Shader shader, const Matrix4f& transformationMatrix, bool omitMaterials) {
    shader.Bind();
    shader.SetModelMatrix(transformationMatrix);

    if (!omitMaterials) {
        material.ApplyToShader(shader);
        material.ApplyTexture(shader);
    }

    vertexObject->Draw();
}

void _SceneObject::SetMaterial(const Material& value) {
    this->material = value;
}

void _SceneObject::SetVertexObject(const VertexObject& value) {
    this->vertexObject = value;
}

Material _SceneObject::GetMaterial(void) const {
    return material;
}