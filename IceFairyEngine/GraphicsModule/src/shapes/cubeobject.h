#ifndef __ice_fairy_cube_object_h__
#define __ice_fairy_cube_object_h__

#include "../shader.h"
#include "../vertexobject.h"

namespace IceFairy {
    /*! \brief Creates a VertexObject as a cube */
    class CubeObject : public _VertexObject {
    public:
        /*! \brief Constructs a VertexObject as a cube
         *
         * \param size The size of the CubeObject
         */
        CubeObject(float size = 1.0f);
    };
}

#endif /* __ice_fairy_cube_object_h__ */