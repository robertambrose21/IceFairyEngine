#ifndef __ice_fairy_scene_node_h__
#define __ice_fairy_scene_node_h__

#include <memory>
#include <vector>

#include "glinclude.h"
#include "math\vector.h"
#include "math\matrix.h"
#include "sceneobject.h"
#include "lighting\pointlight.h"

namespace IceFairy {
    /*! \brief _SceneNode class for use with \ref SceneTree.
     *
     * Scene nodes should not be directly created by instead should be called via \c AddChild
     * either from \ref SceneTree or \ref SceneNode.\n
     * \ref SceneNodes have both a 'real' and a 'relative' position. Real positions are calculated from
     * all transformation matrices applied from root node downwards. Relative positions  only apply transformations
     * from the \ref SceneNode itself.
     * For a full example, see \ref SceneTree.
     */
    class _SceneNode : public std::enable_shared_from_this<_SceneNode> {
    public:
        typedef std::vector<std::shared_ptr<_SceneNode>> SceneNodeList;

        /*! \brief Construct the _SceneNode.
         *
         * \param parent The parent of this _SceneNode.
         */
        _SceneNode(std::shared_ptr<_SceneNode> parent);
        virtual ~_SceneNode();

        /*! \returns A SceneNode child to this SceneNode. */
        std::shared_ptr<_SceneNode> AddChild(void);
        /*! \brief Adds a \ref SceneObject to this SceneNode.
         *
         * \param material The material to apply to the \ref SceneObject.
         * \param vertexObject The vertices of the \ref SceneObject.
         * \returns The newly created \ref SceneObject.
         */
        SceneObject                 AddSceneObject(const Material& material, const VertexObject& vertexObject);
        /*! \brief Removes a \ref SceneObject from this SceneNode.
         *
         * \param index The index of the \ref SceneObject to remove.
         */
        void                        RemoveSceneObject(unsigned int index);
        /*! \brief Removes a child from this SceneNode.
         *
         * \param index The index of the child to remove.
         */
        void                        RemoveChild(unsigned int index);
        /*! \brief Removes all children from this SceneNode. */
        void                        RemoveAllChildren(void);
        /*! \returns The parent of this SceneNode. */
        std::shared_ptr<_SceneNode> GetParent(void) const;
        /*! \brief Draws this SceneNode and all its children.
         *
         * \param shader The \ref Shader to draw the SceneNode on.
         * \param omitMaterials Omit applying materials on this draw call.s
         */
        virtual void                Draw(Shader shader, bool omitMaterials = false);
        /*! \returns All the children for this SceneNode. */
        SceneNodeList               GetChildren(void) const;
        /*! \brief Returns a child at the given index.
         *
         * \param index The index of the child.
         * \returns The child SceneNode.
         */
        std::shared_ptr<_SceneNode> GetChild(unsigned int index);
        /*! \brief Applies a translation transformation to this SceneNode.
         *
         * \param translation The translation vector to apply.
         */
        void                        ApplyTranslation(const Vector3f& translation);
        /*! \brief Applies a scale transformation to this SceneNode.
         *
         * \param scale The scale vector to apply.
         */
        void                        ApplyScale(const Vector3f& scale);
        /*! \brief Applies an X rotation to this SceneNode.
         *
         * \param degrees The amount to rotate the SceneNode by.
         */
        void                        ApplyXRotation(const float& degrees);
        /*! \brief Applies an Y rotation to this SceneNode.
         *
         * \param degrees The amount to rotate the SceneNode by.
         */
        void                        ApplyYRotation(const float& degrees);
        /*! \brief Applies an Z rotation to this SceneNode.
         *
         * \param degrees The amount to rotate the SceneNode by.
         */
        void                        ApplyZRotation(const float& degrees);
        /*! \brief Applies a transformation matrix to this SceneNode.
         *
         * \param transformationMatrix The transformation to apply.
         * \param updateRealPositions A flag for whether this SceneNode should update its real position after applying the
         * transformation, the default is \c true.
         */
        void                        ApplyTransformationMatrix(const Matrix4f& transformationMatrix, bool updateRealPositions = true);
        /*! \returns The relative position of this SceneNode. */
        Vector3f                    GetRelativePosition(void) const;
        /*! \returns The real position of this SceneNode. */
        Vector3f                    GetPosition(void) const;
        /*! \returns The transformation matrix of this SceneNode. */
        Matrix4f                    GetTransformationMatrix(void) const;
        /*! \returns The relative transformation matrix of this SceneNode. */
        Matrix4f                    GetRelativeTransformationMatrix(void) const;
        /*! \brief Instructs the SceneNode to calculate its real position.
         *
         * This can be useful if you wish to defer the real position calculations until later as they can get expensive with many nodes. */
        void                        UpdateRealPositions(void);
        /*! \brief Saves the current transformation matrix to be used again at a later stage. */
        void                        SaveMatrixState(void);
        /*! \brief Resets the transformation matrix to the last saved matrix from \ref SaveMatrixState. */
        void                        ResetMatrixState(void);

        /*! \brief Sets the point light for this SceneNode.
         *
         * \param pointLight The point \ref PointLight to set
         */
        void                        SetPointLight(const std::shared_ptr<PointLight>& pointLight);
        /*! \brief Sets the point light for this SceneNode.
        *
        * \param position The position of the \ref PointLight relative to the SceneNode
        * \param colour The colour of the \ref PointLight
        * \param ambient The ambience of the \ref PointLight
        * \param cAttenuation The constant attenuation of the \ref PointLight
        * \param lAttenuation The linear attenuation of the \ref PointLight
        * \param eAttenuation The exponential attenuation of the \ref PointLight
        */
        void                        SetPointLight(Vector3f position, Colour3f colour, float ambient,
            float cAttenuation, float lAttenuation = 0.0f, float eAttenuation = 0.0f);
        /*! \brief Unsets this \ref PointLight, effectively turning it off. */
        void                        UnsetPointLight(void);
        /*! \returns The \ref PointLight of the SceneNode if it exists. */
        std::shared_ptr<PointLight> GetPointLight(void) const;

    private:
        std::shared_ptr<_SceneNode> parent;
        std::vector<SceneObject>    sceneObjects;
        SceneNodeList               children;
        Vector3f                    relativePosition;
        Vector3f                    position;
        Matrix4f                    relativeTransformationMatrix;
        Matrix4f                    transformationMatrix;
        std::vector<Matrix4f>       matrixStack;
        std::shared_ptr<PointLight> pointLight;
    };

    /*! Short-hand pointer to \ref _SceneNode. These should be used in most cases. */
    typedef std::shared_ptr<_SceneNode> SceneNode;
    typedef _SceneNode::SceneNodeList   SceneNodeList;
}

#endif /* __ice_fairy_scene_node_h__ */
