#ifndef __ice_fairy_drawable_h__
#define __ice_fairy_drawable_h__

#include "shader.h"
#include "bufferobject.h"

#include <vector>
#include <memory>

namespace IceFairy {
    /*! \brief Drawable interface for drawing single frames via the currently loaded graphics engine.
     *
     * \ref DrawFrame is called once for each \ref BufferObject residing on the \ref Drawable.
     * Any number of \ref BufferObject's may be attached to the \ref Drawable but should be kept to a minimum.
     *
     * \ref DrawFrame should never be called directly but instead attached to the \ref GraphicsModule which calls it for us.
     * Sample usage:
     * \code{.cpp}
     * class GraphicsApplication : public IceFairy::Application, public IceFairy::Drawable {
     * public:
     *     void Initialise(void) {
     *         graphicsModule->CreateWindow(1024, 768, "Graphics Application");
     *         graphicsModule->AddDrawable(shared_from_this());
     *         graphicsModule->StartMainLoop(*this);
     *
     *         AddBufferObject("colourBuffer", IceFairy::BufferObject::COLOUR, shader, 1024, 768);
     *     }
     *
     *     void DrawFrame(Shader shader, std::shared_ptr<BufferObject> bufferObject) {
     *         // ... Draw stuff within the current buffer here ...
     *     }
     * };
     * \endcode
     */
    class Drawable {
    public:
        typedef std::vector<std::shared_ptr<BufferObject>> BufferObjectList;

        Drawable();

        /*! \brief Draws the current frame within the current buffer. This should never be called directly.
         *
         * \param shader The shader associated with the current \ref BufferObject
         * \param bufferObject The current \ref BufferObject
         */
        virtual void                    DrawFrame(Shader shader, std::shared_ptr<BufferObject> bufferObject = nullptr) = 0;

        /*! \brief Draws all the available buffers
         *
         * \param width How wide to draw the \ref BufferObject's
         * \param height How high to draw the \ref BufferObject's
         */
        virtual void                    DrawBuffers(unsigned int width, unsigned int height);
        /*! \brief Adds a \ref BufferObject to this Drawable
         *
         * \param bufferObject The \ref BufferObject to add
         * \returns The list id of the \ref BufferObject
         */
        unsigned int                    AddBufferObject(std::shared_ptr<BufferObject> bufferObject);
        /*! \brief Adds a new \ref BufferObject to this Drawable
        *
        * \param name The name of the \ref BufferObject
        * \param type The type of the \ref BufferObject
        * \param shader The shader of the \ref BufferObject
        * \param width The width of the \ref BufferObject
        * \param height The height of the \ref BufferObject
        * \returns The list id of the \ref BufferObject
        */
        unsigned int                    AddBufferObject(std::string name, BufferObject::Type type, Shader shader, unsigned int width, unsigned int height);

        /*! \brief Gets a \ref BufferObject at the specified index
         *
         * \param index The index of the \ref BufferObject
         * \returns the \ref BufferObject
         */
        std::shared_ptr<BufferObject>   GetBufferObject(unsigned int index);
        /*! \returns A list of all \ref BufferObject's */
        BufferObjectList                GetBufferObjects(void);

    private:
        BufferObjectList    bufferObjects;
        unsigned int        currentBufferIndex;
    };

    typedef std::vector<std::shared_ptr<Drawable>> DrawablesList;
}

#endif /* __ice_fairy_drawable_h__ */