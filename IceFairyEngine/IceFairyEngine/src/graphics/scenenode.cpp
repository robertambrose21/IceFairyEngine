#include "scenenode.h"

using namespace IceFairy;

_SceneNode::_SceneNode(std::shared_ptr<_SceneNode> parent)
    : parent(parent),
      relativePosition(Vector3f(0.0f, 0.0f, 0.0f)),
      position(Vector3f(0.0f, 0.0f, 0.0f)),
      transformationMatrix(Matrix4f::Identity()),
      relativeTransformationMatrix(Matrix4f::Identity()),
      pointLight(nullptr)
{
    if (parent != nullptr)
        transformationMatrix = parent->GetTransformationMatrix();
}

_SceneNode::~_SceneNode()
{ }

std::shared_ptr<_SceneNode> _SceneNode::AddChild(void) {
    auto child = std::shared_ptr<_SceneNode>(new _SceneNode(shared_from_this()));
    children.push_back(child);
    return child;
}

SceneObject _SceneNode::AddSceneObject(const Material& material, const VertexObject& vertexObject) {
    auto sceneObject = std::shared_ptr<_SceneObject>(new _SceneObject());
    sceneObject->SetMaterial(material);
    sceneObject->SetVertexObject(vertexObject);
    sceneObjects.push_back(sceneObject);

    return sceneObject;
}

void _SceneNode::RemoveSceneObject(unsigned int index) {
    sceneObjects.erase(sceneObjects.begin() + index);
}

void _SceneNode::RemoveChild(unsigned int index) {
    children.erase(children.begin() + index);
}

void _SceneNode::RemoveAllChildren(void) {
    children.clear();
}

std::shared_ptr<_SceneNode> _SceneNode::GetParent(void) const {
    return parent;
}

void _SceneNode::Draw(Shader shader, bool omitMaterials) {
    for (std::shared_ptr<_SceneNode>& child : children) {
        child->Draw(shader, omitMaterials);
    }

    for (SceneObject& sceneObject : sceneObjects) {
        sceneObject->Draw(shader, transformationMatrix);
    }
}

_SceneNode::SceneNodeList _SceneNode::GetChildren(void) const {
    return children;
}

std::shared_ptr<_SceneNode> _SceneNode::GetChild(unsigned int index) {
    return children[index];
}

void _SceneNode::ApplyTranslation(const Vector3f& translation) {
    ApplyTransformationMatrix(Matrix4f::Translate(translation));
}

void _SceneNode::ApplyScale(const Vector3f& scale) {
    ApplyTransformationMatrix(Matrix4f::Scale(scale));
}

void _SceneNode::ApplyXRotation(const float& degrees) {
    ApplyTransformationMatrix(Matrix4f::Rotate(degrees, 1, 0, 0));
}

void _SceneNode::ApplyYRotation(const float& degrees) {
    ApplyTransformationMatrix(Matrix4f::Rotate(degrees, 0, 1, 0));
}

void _SceneNode::ApplyZRotation(const float& degrees) {
    ApplyTransformationMatrix(Matrix4f::Rotate(degrees, 0, 0, 1));
}

void _SceneNode::ApplyTransformationMatrix(const Matrix4f& transformationMatrix, bool updateRealPositions) {
    relativeTransformationMatrix = relativeTransformationMatrix * transformationMatrix;
    relativePosition = (relativeTransformationMatrix * Vector4f(0.0f, 0.0f, 0.0f, 1.0f)).ToVector3();

    if (updateRealPositions) {
        UpdateRealPositions();

        for (SceneNode& child : children) {
            child->UpdateRealPositions();
        }
    }
}

Vector3f _SceneNode::GetRelativePosition(void) const {
    return relativePosition;
}

Vector3f _SceneNode::GetPosition(void) const {
    return position;
}

Matrix4f _SceneNode::GetTransformationMatrix(void) const {
    return transformationMatrix;
}

Matrix4f _SceneNode::GetRelativeTransformationMatrix(void) const {
    return relativeTransformationMatrix;
}

void _SceneNode::UpdateRealPositions(void) {
    if (parent != nullptr) {
        transformationMatrix = parent->GetTransformationMatrix() * relativeTransformationMatrix;
        position = relativePosition + parent->GetPosition();
    }
    else {
        transformationMatrix = relativeTransformationMatrix;
        position = relativePosition;
    }
}

void _SceneNode::SaveMatrixState(void) {
    matrixStack.push_back(relativeTransformationMatrix);
}

void _SceneNode::ResetMatrixState(void) {
    if (!matrixStack.empty()) {
        relativeTransformationMatrix = matrixStack.back();
        matrixStack.pop_back();
    }

    UpdateRealPositions();
}

void _SceneNode::SetPointLight(const std::shared_ptr<PointLight>& pointLight) {
    this->pointLight = pointLight;
}

void _SceneNode::SetPointLight(Vector3f position, Colour3f colour, float ambient,
    float cAttenuation, float lAttenuation, float eAttenuation)
{
    SetPointLight(std::shared_ptr<PointLight>(new PointLight(position, colour, ambient, cAttenuation, lAttenuation, eAttenuation)));
}

void _SceneNode::UnsetPointLight(void) {
    SetPointLight(nullptr);
}

std::shared_ptr<PointLight> _SceneNode::GetPointLight(void) const {
    return pointLight;
}