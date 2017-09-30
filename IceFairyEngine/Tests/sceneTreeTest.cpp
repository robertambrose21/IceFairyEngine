#include "sceneTreeTest.h"

IceFairy::SceneNode GetRootSceneNode(void) {
    IceFairy::SceneTree sceneTree;
    return sceneTree.AddChild();
}

/////////////////////////////////////////////////////////////////
////////////////////////// BEGIN TESTS //////////////////////////
/////////////////////////////////////////////////////////////////
TEST(SceneTree, AddChild) {
    IceFairy::SceneTree sceneTree;

    IceFairy::SceneNode node1 = sceneTree.AddChild();
    IceFairy::SceneNode node2 = sceneTree.AddChild();

    EXPECT_EQ(2, sceneTree.GetChildren().size());
    EXPECT_EQ(node1, sceneTree.GetChild(0));
    EXPECT_EQ(node2, sceneTree.GetChild(1));
}

TEST(SceneTree, RemoveChild) {
    IceFairy::SceneTree sceneTree;

    IceFairy::SceneNode node1 = sceneTree.AddChild();
    IceFairy::SceneNode node2 = sceneTree.AddChild();
    IceFairy::SceneNode node3 = sceneTree.AddChild();

    sceneTree.RemoveChild(1);

    EXPECT_EQ(2, sceneTree.GetChildren().size());
    EXPECT_EQ(node3, sceneTree.GetChild(1));

    sceneTree.RemoveAllChildren();

    EXPECT_TRUE(sceneTree.GetChildren().empty());
}

TEST(SceneTree, Draw) {
    IceFairy::SceneTree sceneTree;
}

TEST(SceneNode, AddChild) {
    IceFairy::SceneNode root = GetRootSceneNode();

    IceFairy::SceneNode node1 = root->AddChild();
    IceFairy::SceneNode node2 = root->AddChild();

    EXPECT_EQ(2, root->GetChildren().size());
    EXPECT_EQ(node1, root->GetChild(0));
    EXPECT_EQ(node2, root->GetChild(1));
}

TEST(SceneNode, RemoveChild) {
    IceFairy::SceneNode root = GetRootSceneNode();

    IceFairy::SceneNode node1 = root->AddChild();
    IceFairy::SceneNode node2 = root->AddChild();
    IceFairy::SceneNode node3 = root->AddChild();

    root->RemoveChild(1);

    EXPECT_EQ(2, root->GetChildren().size());
    EXPECT_EQ(node3, root->GetChild(1));

    root->RemoveAllChildren();

    EXPECT_TRUE(root->GetChildren().empty());
}

TEST(SceneNode, GetParent) {
    IceFairy::SceneNode root = GetRootSceneNode();

    IceFairy::SceneNode node1 = root->AddChild();
    IceFairy::SceneNode node2 = node1->AddChild();

    EXPECT_EQ(root, node1->GetParent());
    EXPECT_EQ(node1, node2->GetParent());
}

TEST(SceneNode, ApplyTranslation) {
    IceFairy::SceneNode root = GetRootSceneNode();

    EXPECT_EQ(IceFairy::Vector3f(0, 0, 0), root->GetPosition());

    root->ApplyTranslation(IceFairy::Vector3f(1, 2, 3));

    EXPECT_EQ(IceFairy::Vector3f(1, 2, 3), root->GetPosition());
}

TEST(SceneNode, ApplyRotation) {
    IceFairy::SceneNode root = GetRootSceneNode();\

    root->SaveMatrixState();
    root->ApplyXRotation(90);
    root->ApplyTranslation(IceFairy::Vector3f(0, 0, 1));

    V3M_FuzzyFloatMatch(IceFairy::Vector3f(0, 1, 0), root->GetPosition());

    root->ResetMatrixState();

    root->SaveMatrixState();
    root->ApplyZRotation(90);
    root->ApplyTranslation(IceFairy::Vector3f(0, 1, 0));

    V3M_FuzzyFloatMatch(IceFairy::Vector3f(1, 0, 0), root->GetPosition());

    root->ResetMatrixState();

    root->SaveMatrixState();
    root->ApplyYRotation(90);
    root->ApplyTranslation(IceFairy::Vector3f(1, 0, 0));

    V3M_FuzzyFloatMatch(IceFairy::Vector3f(0, 0, 1), root->GetPosition());

    root->ResetMatrixState();
}

TEST(SceneNode, ApplyScale) {
    IceFairy::SceneNode root = GetRootSceneNode();
    root->ApplyScale(IceFairy::Vector3f(2, 3, 4));
    root->ApplyTranslation(IceFairy::Vector3f(1, 2, 3));

    EXPECT_EQ(IceFairy::Vector3f(2, 6, 12), root->GetPosition());
}

TEST(SceneNode, GetPosition) {
    IceFairy::SceneNode root = GetRootSceneNode();

    IceFairy::SceneNode node1 = root->AddChild();

    IceFairy::SceneNode node2 = node1->AddChild();
    node1->ApplyTranslation(IceFairy::Vector3f(10, 5, 0));
    node2->ApplyTranslation(IceFairy::Vector3f(-10, 0, 0));

    node1->ApplyZRotation(90.0f);
    node1->ApplyTranslation(IceFairy::Vector3f(1, 0, 0));

    V3M_FuzzyFloatMatch(IceFairy::Vector3f(10, 4, 0), node1->GetPosition());
    V3M_FuzzyFloatMatch(IceFairy::Vector3f(0, 4, 0), node2->GetPosition());
    V3M_FuzzyFloatMatch(IceFairy::Vector3f(-10, 0, 0), node2->GetRelativePosition());
}