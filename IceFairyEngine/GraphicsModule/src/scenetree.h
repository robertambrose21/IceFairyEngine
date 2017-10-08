#ifndef __ice_fairy_scene_tree_h__
#define __ice_fairy_scene_tree_h__

#include <vector>

#include "glinclude.h"
#include "scenenode.h"
#include "lighting\pointlight.h"

namespace IceFairy {
    /*! \brief SceneTree class for creating and drawing scenes.
     *
     * Example use:
     * \code{.cpp}
     * IceFairy::SceneTree sceneTree;
     * IceFairy::SceneNode sceneNode = sceneTree.AddChild();
     * sceneNode->ApplyTranslation(IceFairy::Vector3f(1, 0, 0));
     * sceneNode->ApplyZRotation(90.0f);
     *
     * IceFairy::SceneNode sn2 = sceneNode->AddChild();
     *
     * sceneNode->ApplyTranslation(IceFairy::Vector3f(10, 5, 0));
     * sn2->ApplyTranslation(IceFairy::Vector3f(-10, 0, 0));
     *
     * IceFairy::Logger::Print("sceneNode: %s\n", sceneNode->GetRelativePosition().Str().c_str());
     * IceFairy::Logger::Print("sn2: [real: %s][relative: %s]\n", sn2->GetPosition().Str().c_str(), sn2->GetRelativePosition().Str().c_str());
     * \endcode
     * When drawing:
     * \code{.cpp}
     * sceneNode->SaveMatrixState();
     * sceneNode->ApplyTranslation(IceFairy::Vector3f(2, 0, 0));
     * 
     * sn2->ApplyZRotation(-1.0f);
     * 
     * // Draw stuff...
     * 
     * sceneNode->ResetMatrixState();
     * \endcode
     */
    class SceneTree {
    public:
        typedef std::vector<std::shared_ptr<PointLight>> PointLightList;

        SceneTree();

        /*! \returns A SceneNode child to this SceneTree. */
        SceneNode       AddChild(void);
        /*! \brief Removes a child from this SceneTree.
         *
         * \param index The index of the child to remove.
         */
        void            RemoveChild(unsigned int index);
        /*! \brief Removes all children from this SceneTree. */
        void            RemoveAllChildren(void);
        /*! \brief Draws all the children of this SceneTree.
         *
         * \param shader The \ref Shader to draw the SceneTree on.
         * \param omitMaterials Omit applying materials on this draw call.
         */
        void            Draw(Shader shader, bool omitMaterials = false);
        /*! \returns All the children for this SceneTree. */
        SceneNodeList   GetChildren(void) const;
        /*! \brief Returns a child at the given index.
         *
         * \param index The index of the child.
         * \returns The child SceneNode.
         */
        SceneNode       GetChild(unsigned int index) const;
        /*! \returns The root \ref SceneNode. */
        SceneNode       GetRoot(void);

        /*! \returns a list of all \ref PointLight's on the SceneTree. */
        PointLightList  GetPointLights(void);
        /*! \brief Draws the \ref PointLight's on the SceneTree.
         *
         * \param shader The \ref Shader to draw the \ref PointLight's with
         */
        void            DrawPointLights(Shader shader);

    private:
        void            PopulatePointLights(PointLightList& pointLights, SceneNode node);
        void            DrawPointLights(Shader shader, SceneNode node, unsigned int& numLightsDrawn);
        std::string     GetPointLightShaderName(const unsigned int& index, std::string append);

        SceneNodeList   children;
        SceneNode       root;
    };
}

#endif /* __ice_fairy_scene_tree_h__ */
