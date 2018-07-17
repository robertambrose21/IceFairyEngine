#ifndef __ice_fairy_vertex_object_h__
#define __ice_fairy_vertex_object_h__

#include <vector>

#include "opengl/glinclude.h"
#include "math/vector.h"
#include "core/utilities/logger.h"
#include "core/utilities/icexception.h"

namespace IceFairy {
	/*! \brief Thrown when the number of points and normals specified differs. */
	class InvalidPointSpecificationException : public ICException {
	public:
		/*! \internal */
		InvalidPointSpecificationException()
			: ICException("The number of supplied normals and points is different.")
		{ }
	};

	/*! \brief Basic Vertex struct for assigning the position, normal and texture coordinate of a vertex */
	typedef struct _vertex {
		/*! \brief Create a \ref Vertex with a position and a normal */
		_vertex(Vector3f position, Vector3f normal) {
			this->position[0] = position.x;
			this->position[1] = position.y;
			this->position[2] = position.z;

			this->normal[0] = normal.x;
			this->normal[1] = normal.y;
			this->normal[2] = normal.z;
		}
		/*! \brief Create a \ref Vertex with a position, normal and texcoord */
		_vertex(Vector3f position, Vector3f normal, Vector2f texcoord) {
			this->position[0] = position.x;
			this->position[1] = position.y;
			this->position[2] = position.z;

			this->normal[0] = normal.x;
			this->normal[1] = normal.y;
			this->normal[2] = normal.z;

			this->texcoord[0] = texcoord.x;
			this->texcoord[1] = texcoord.y;
		}

		GLfloat position[3];
		GLfloat normal[3];
		GLfloat texcoord[2];
	} Vertex;

    /*! \brief Vertex object class for specifying and creating drawable vertices. Used in \ref SceneObject.
     *
     * Sample usage:
     * \code{.cpp}
     * unsigned int indices[6] = {
     *    0, 1, 2, 2, 3, 0
     * };
     *
     * IceFairy::Vector3f vertices[4] = {
     *    IceFairy::Vector3f(1.0f, 0.0f, 0.0f),
     *    IceFairy::Vector3f(1.0f, 1.0f, 0.0f),
     *    IceFairy::Vector3f(0.0f, 1.0f, 0.0f),
     *    IceFairy::Vector3f(0.0f, 0.0f, 0.0f)
     * };
     *
     * IceFairy::Vector3f normals[4] = {
     *    IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
     *    IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
     *    IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
     *    IceFairy::Vector3f(0.0f, 0.0f, 1.0f)
     * };
     *
     * auto vo = IceFairy::CreateVertexObject(indices, 6, vertices, normals, 4);
     * \endcode
     */
    class _VertexObject {
    public:
        /*! \brief Create an empty vertex object. */
        _VertexObject();
        /*! \brief Create a vertex object with the given indices, points, and normals.
         *
         * \param indices The indices of this vertex object.
         * \param numIndices The number of indices on this vertex object.
         * \param points The points of this vertex object.
         * \param normals The normals of this vertex object.
         * \param numPoints The number of points on this vertex object.
         * \throws InvalidPointSpecificationException
         */
        _VertexObject(
            unsigned int* indices,
            unsigned int numIndices, 
            Vector3f* points,
            Vector3f* normals,
            unsigned int numPoints
        );
        /*! \brief Create a vertex object with the given indices, points, and normals.
        *
        * \param indices The indices of this vertex object.
        * \param numIndices The number of indices on this vertex object.
        * \param points The points of this vertex object.
        * \param normals The normals of this vertex object.
        * \param texcoords The texture coordinates of this vertex object.
        * \param numPoints The number of points on this vertex object.
        * \throws InvalidPointSpecificationException
        */
        _VertexObject(
            unsigned int* indices, 
            unsigned int numIndices,
            Vector3f* points,
            Vector3f* normals,
            Vector2f* texcoords,
            unsigned int numPoints
        );
        /*! \brief Create a vertex object with the given indices, points, and normals.
         *
         * \param indices The indices of this vertex object.
         * \param points The points of this vertex object.
         * \param normals The normals of this vertex object.
         */
        _VertexObject(
            const std::vector<unsigned int>& indices,
            const std::vector<Vector3f>& points,
            const std::vector<Vector3f>& normals
        );
        virtual ~_VertexObject();

        /*! \brief Draws a single frame of this vertex object. */
        void    Draw(void);

    protected:
        void    BuildIndices(const std::vector<unsigned int>& indices);
        void    BuildIndices(unsigned int* indices, unsigned int numIndices);
        void    BuildVertices(const std::vector<Vertex>& vertices);

        GLuint          ibo;
        GLuint          vbo;
        unsigned int    numIndices;
    };

    typedef std::shared_ptr<_VertexObject> VertexObject;

    /*! \brief Short-hand for creating a vertex object.
     *
     * \param indices The indices of this vertex object.
     * \param numIndices The number of indices on this vertex object.
     * \param points The points of this vertex object.
     * \param normals The normals of this vertex object.
     * \param numPoints The number of points on this vertex object.
     * \returns The newly created vertex object.
     */
    static inline VertexObject CreateVertexObject(
        unsigned int* indices, 
        unsigned int numIndices, 
        Vector3f* points, 
        Vector3f* normals,
        unsigned int numPoints)
    {
        return std::shared_ptr<_VertexObject>(new _VertexObject(indices, numIndices, points, normals, numPoints));
    }

    /*! \brief Short-hand for creating a vertex object.
    *
    * \param indices The indices of this vertex object.
    * \param numIndices The number of indices on this vertex object.
    * \param points The points of this vertex object.
    * \param normals The normals of this vertex object.
    * \param texcoords The texture coordinates of this vertex object.
    * \param numPoints The number of points on this vertex object.
    * \returns The newly created vertex object.
    */
    static inline VertexObject CreateVertexObject(
        unsigned int* indices,
        unsigned int numIndices,
        Vector3f* points,
        Vector3f* normals,
        Vector2f* texcoords,
        unsigned int numPoints)
    {
        return std::shared_ptr<_VertexObject>(new _VertexObject(indices, numIndices, points, normals, texcoords, numPoints));
    }

    /*! \brief Short-hand for creating a vertex object.
     *
     * \param indices The indices of this vertex object.
     * \param points The points of this vertex object.
     * \param normals The normals of this vertex object.
     * \returns The newly created vertex object.
     */
    static inline VertexObject CreateVertexObject(
        const std::vector<unsigned int>& indices,
        const std::vector<Vector3f>& points,
        const std::vector<Vector3f>& normals)
    {
        return std::shared_ptr<_VertexObject>(new _VertexObject(indices, points, normals));
    }
}

#endif /* __ice_fairy_vertex_object_h__ */