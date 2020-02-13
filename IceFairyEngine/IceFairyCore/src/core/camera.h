#ifndef __ice_fairy_camera_h__
#define __ice_fairy_camera_h__

#include "../math/vector.h"
#include "../math/matrix.h"

namespace IceFairy {
    /*! \brief Camera for navigating a scene.
     *
     * Sample usage:
     * \code{.cpp}
     * IceFairy::Camera camera(
     *     IceFairy::Vector3f(3, 0, 3),
     *     IceFairy::Vector3f(0, 0, 0),
     *     IceFairy::Vector3f(0, 0, 1));
     *
     * IceFairy::Matrix4f viewMatrix = camera.GetViewMatrix();
     */
    class Camera {
    public:
        Camera() { }
        /*! Constructs the camera.
         *
         * \param eye The eye point of the camera
         * \param lookAt The centre 'look at' point of the camera
         * \param up The up vector of the camera
         */
        Camera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up);

        /*! \param eye Sets the cameras eye */
        void			SetEye(const Vector3f& eye);
		Vector3f		GetEye(void) const;
        /*! \param lookAt Sets the cameras look at */
        void			SetLookAt(const Vector3f& lookAt);
		Vector3f		GetLookAt(void) const;
        /*! \param up Sets the cameras up vector */
        void			SetUp(const Vector3f& up);
		Vector3f		GetUp(void) const;
        /*! \brief Updates the cameras view matrix with the current eye, lookAt and up vectors */
        virtual void	Update(long timeSinceLastFrame);

        /*! \returns The cameras view matrix */
        Matrix4f		GetViewMatrix(void) const;

    protected:
        Vector3f    eye;
        Vector3f    lookAt;
        Vector3f    up;
        Matrix4f    viewMatrix;
    };
}

#endif /* __ice_fairy_camera_h__ */