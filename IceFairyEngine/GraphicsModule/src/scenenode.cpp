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
    auto child = std::make_shared<_SceneNode>(shared_from_this());
    children.push_back(child);
    return child;
}

SceneObject _SceneNode::AddSceneObject(const Material& material, const VertexObject& vertexObject) {
    auto sceneObject = std::make_shared<_SceneObject>();
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

void _SceneNode::ApplyTransformationMatrix(const Matrix4f& matrix, bool updateRealPositions) {
	relativeTransformationMatrix = matrix * relativeTransformationMatrix;
    relativePosition = (relativeTransformationMatrix * Vector4f(0.0f, 0.0f, 0.0f, 1.0f)).ToVector3();

    if (updateRealPositions) {
        UpdateRealPositions();

        for (SceneNode& child : children) {
            child->UpdateRealPositions();
        }
    } else {
		transformationMatrix = relativeTransformationMatrix;
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

std::shared_ptr<PointLight> _SceneNode::SetPointLight(const std::shared_ptr<PointLight>& value) {
	return SetPointLight(
		value->GetPosition(),
		value->GetColour(),
		value->GetAmbient(),
		value->GetConstantAttentuation(),
		value->GetLinearAttentuation(),
		value->GetExponentialAttentuation()
	);
}

std::shared_ptr<PointLight> _SceneNode::SetPointLight(Vector3f p, Colour3f c, float a,  float cAtt, float lAtt, float eAtt)
{
	if (!this->pointLight) {
		this->pointLight = std::make_shared<PointLight>(p, c, a, cAtt, lAtt, eAtt);
	} else {
		this->pointLight->SetPosition(p);
		this->pointLight->SetColour(c);
		this->pointLight->SetAmbient(a);
		this->pointLight->SetConstantAttentuation(cAtt);
		this->pointLight->SetLinearAttentuation(lAtt);
		this->pointLight->SetExponentialAttentuation(eAtt);
	}

	return this->pointLight;
}

void _SceneNode::UnsetPointLight(void) {
    SetPointLight(nullptr);
}

std::shared_ptr<PointLight> _SceneNode::GetPointLight(void) const {
    return pointLight;
}