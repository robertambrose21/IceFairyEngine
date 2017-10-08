#ifndef __ice_fairy_buffer_object_h__
#define __ice_fairy_buffer_object_h__

#include <string>
#include <functional>

#include "glinclude.h"
#include "shader.h"

#include "core\utilities\icexception.h"

namespace IceFairy {
    /*! \brief Thrown if an error occurs while creating the buffer object */
    class BufferObjectException : public ICException {
    public:
        BufferObjectException(std::string message)
            : ICException("A BufferObjectException occurred: " + message)
        { }
    };

    /*! \brief Drawable buffers for combining scenes/effects.
     *
     * Generally BufferObjects should be attached to a \ref Drawable and the draw code
     * executed within a call to \ref Drawable::DrawFrame.
     * See \ref Drawable for sample usage.
     */
    class BufferObject : public std::enable_shared_from_this<BufferObject> {
    public:
		typedef std::function<void(Shader, std::shared_ptr<BufferObject>)> DrawFunction;

        /*! \brief Possible buffer types */
        enum Type {
            /*! \brief Depth buffer */
            DEPTH,
            /*! \brief Colour buffer */
            COLOUR
        };

        BufferObject();
        /*! \brief Constructs the buffer object 
         *
         * \param name The name of the \ref BufferObject
         * \param type The type of the \ref BufferObject
         * \param shader The shader of the \ref BufferObject
         * \param width The width of the \ref BufferObject
         * \param height The height of the \ref BufferObject
         */
        BufferObject(std::string name, Type type, Shader shader, DrawFunction drawFunction, unsigned int width, unsigned int height);
        virtual ~BufferObject();

        /*! \brief Does draw work within this BufferObject
         *
         * \param drawWidth The original width to revert to when done drawing
         * \param drawHeight The original height to revert to when done drawing
         * \param doDrawWork The draw function to do within this BufferObject
         */
		void            DrawInBuffer(unsigned int drawWidth, unsigned int drawHeight);
        /*! \brief Recreates the BufferObject
         *
         * \param width The new width of the BufferObject
         * \param height The new height of the BufferObject
         */
        void            RecreateBuffer(unsigned int width, unsigned int height);

        /*! \param width Sets the width of the BufferObject */
        void            SetWidth(const unsigned int& width);
        /*! \param height Sets the height of the BufferObject */
        void            SetHeight(const unsigned int& height);

        /*! \returns The type of the BufferObject */
        Type            GetType(void) const;
        /*! \returns The width of the BufferObject */
        unsigned int    GetWidth(void) const;
        /*! \returns The height of the BufferObject */
        unsigned int    GetHeight(void) const;
        /*! \returns The created texture handle of the BufferObject */
        GLuint          GetTextureID(void) const;
        /*! \returns The shader of the BufferObject */
        Shader          GetShader(void) const;
        /*! \returns The name of the BufferObject */
        std::string     GetName(void) const;

    protected:
        void            CreateBuffer(void);
        void            CreateDepthBuffer(void);
        void            CreateColourBuffer(void);

        void            CheckFrameBufferStatus(void);

		std::string		name;

        Type            type;
        unsigned int    width;
        unsigned int    height;

        GLuint          framebufferID;
        GLuint          framebufferTextureID;
		GLuint			rboDepthStencilID;

        Shader          shader;
		DrawFunction	drawFunction;
    };
}

#endif /* __ice_fairy_buffer_object_h__ */