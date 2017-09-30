#ifndef __ice_fairy_scene_object_h__
#define __ice_fairy_scene_object_h__

#include "material.h"
#include "vertexobject.h"
#include "shader.h"

namespace IceFairy {
    /*! \brief A drawable object attached to a \ref SceneNode.
     *
     * Scene objects should not be directly created and should instead be called via
     * \c AddSceneObject from \ref SceneNode.
     * Sample usage:
     * \code{.cpp}
     * auto sceneObject = sceneNode->AddSceneObject(
     *      IceFairy::CreateMaterial(
     *          IceFairy::Colour4f(1.0f, 0.0f, 0.0f, 1.0f),
     *          IceFairy::Colour4f(0.2f, 0.0f, 0.0f, 1.0f),
     *          IceFairy::Colour4f(1.0f, 1.0f, 1.0f, 1.0f),
     *          5.0f),
     *      IceFairy::CreateVertexObject(indices, 6, vertices, normals, 4),
     *      shader);
     * \endcode
     */
    class _SceneObject {
    public:
        /*! \brief Construct the scene object. */
        _SceneObject();

        /*! \brief Draws the scene object.
         *
         * \param shader The \ref Shader to draw the SceneObject on.
         * \param transformationMatrix The transformation matrix, passed in from the parent \ref SceneNode.
         * \param omitMaterials Omit applying materials on this draw call.
         */
        void		Draw(Shader shader, const Matrix4f& transformationMatrix, bool omitMaterials = false);
        /*! \brief Sets the scene objects material.
         *
         * \param material The material to set.
         */
        void		SetMaterial(const Material& material);
        /*! \brief Sets the scene objects vertex object.
        *
        * \param vertexObject The vertex object to set.
        */
        void		SetVertexObject(const VertexObject& vertexObject);
        /*! \returns The scene objects material */
		Material	GetMaterial(void) const;

    protected:
        Material		material;
        VertexObject    vertexObject;
        Shader          shader;
    };

    typedef std::shared_ptr<_SceneObject> SceneObject;
}

#endif /* __ice_fairy_scene_object_h__ */