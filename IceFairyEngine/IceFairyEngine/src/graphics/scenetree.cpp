#include "scenetree.h"

using namespace IceFairy;

SceneTree::SceneTree()
    : root(SceneNode(new _SceneNode(nullptr)))
{ }

SceneNode SceneTree::AddChild(void) {
    auto child = root->AddChild();
    children.push_back(child);
    return child;
}

void SceneTree::RemoveChild(unsigned int index) {
    children.erase(children.begin() + index);
}

void SceneTree::RemoveAllChildren(void) {
    children.clear();
}

void SceneTree::Draw(Shader shader, bool omitMaterials) {
    for (SceneNode& child : children) {
        child->Draw(shader, omitMaterials);
    }
}

SceneNodeList SceneTree::GetChildren(void) const {
    return children;
}

SceneNode SceneTree::GetChild(unsigned int index) const {
    return children[index];
}

SceneNode SceneTree::GetRoot(void) {
    return root;
}

SceneTree::PointLightList SceneTree::GetPointLights(void) {
    PointLightList pointLights;

    for (SceneNode& child : children) {
        PopulatePointLights(pointLights, child);
    }

    return pointLights;
}

void SceneTree::PopulatePointLights(PointLightList& pointLights, SceneNode node) {
    for (SceneNode& child : node->GetChildren()) {
        std::shared_ptr<PointLight> pointLight = child->GetPointLight();

        if (pointLight != nullptr) {
            pointLights.push_back(pointLight);
        }

        PopulatePointLights(pointLights, child);
    }
}

void SceneTree::DrawPointLights(Shader shader) {
    unsigned int numPointLights = 0;

    DrawPointLights(shader, root, numPointLights);

    shader.SetUniform1i("numPointLights", numPointLights);
}

void SceneTree::DrawPointLights(Shader shader, SceneNode node, unsigned int& numLightsDrawn) {
    if (numLightsDrawn > ICE_FAIRY_MAX_POINT_LIGHTS) {
        throw MaxPointLightsExceededException();
    }

    for (SceneNode& child : node->GetChildren()) {
        DrawPointLights(shader, child, numLightsDrawn);
    }

    std::shared_ptr<PointLight> pointLight = node->GetPointLight();

    if (pointLight != nullptr) {
        pointLight->Draw(shader, numLightsDrawn++, node->GetPosition());
    }
}

std::string SceneTree::GetPointLightShaderName(const unsigned int& index, std::string append) {
    return "pointLights[" + std::to_string(index) + "]." + append;
}