#ifndef __ice_fairy_material_h__
#define __ice_fairy_material_h__

#include <string>

#include "glinclude.h"
#include "shader.h"
#include "stbi/stb_image.h"
#include "../math/colour.h"
#include "../core/utilities/resource.h"
#include "../core/utilities/icexception.h"

namespace IceFairy {
    /*! \brief Thrown when the texture unit exceeds the limit (31). */
    class InvalidTextureUnitException : public ICException {
    public:
        /*! \internal */
        InvalidTextureUnitException()
            : ICException("The specified texture unit is over the limit, please supply a unit within the range of (0 - 31).")
        { }
    };

    /*! \brief Material class for applying fragment shader properties.
     *
     * Materials have default 'hooks' setup to access shader properties. e.g. the variable
     * \c diffuseColour should be used in the fragment shader to pick up the materials base colour.
     * These hooks can be changed via calls to the material's hook setting methods such as \ref SetDiffuseShaderHook.
     * Sample usage:
     * \code{.cpp}
     * IceFairy::Material red = IceFairy::CreateMaterial(IceFairy::Colour4f(1.0f, 0.0f, 0.0f, 1.0f));
     * \endcode
     */
    class Material : public std::enable_shared_from_this<Material> {
    public:
        /*! \brief the default diffuse colour shader hook, \c diffuseColour */
        static const std::string    DEFAULT_DIFFUSE_HOOK;
        /*! \brief the default diffuse colour shader hook, \c texSampler */
        static const std::string    DEFAULT_SAMPLER_HOOK;
		/*! \brief the default ambient colour shader hook, \c ambientColour */
		static const std::string    DEFAULT_AMBIENT_HOOK;
		/*! \brief the default specular colour shader hook, \c specularColour */
		static const std::string    DEFAULT_SPECULAR_HOOK;
		/*! \brief the default intensity shader hook, \c intensity */
		static const std::string    DEFAULT_INTENSITY_HOOK;

		// TODO: move
        ///*! \brief a red diffuse material. */
        //static const Material       DIFFUSE_RED;
        ///*! \brief a green diffuse material. */
        //static const Material       DIFFUSE_GREEN;
        ///*! \brief a blue diffuse material. */
        //static const Material       DIFFUSE_BLUE;
        ///*! \brief a yellow diffuse material. */
        //static const Material       DIFFUSE_YELLOW;
        ///*! \brief a purple diffuse material. */
        //static const Material       DIFFUSE_PURPLE;
        ///*! \brief a teal diffuse material. */
        //static const Material       DIFFUSE_TEAL;
        ///*! \brief a black diffuse material. */
        //static const Material       DIFFUSE_BLACK;
        ///*! \brief a white diffuse material. */
        //static const Material       DIFFUSE_WHITE;

		class Builder : public std::enable_shared_from_this<Builder> {
		public:
			Builder();

			std::shared_ptr<Builder> withDiffuse(const Colour4f& diffuse, const std::string& hook = DEFAULT_DIFFUSE_HOOK);
			std::shared_ptr<Builder> withTexture(const std::string& filename, const std::string& hook = DEFAULT_SAMPLER_HOOK);
			std::shared_ptr<Builder> withAmbient(const Colour4f& ambient, const std::string& hook = DEFAULT_AMBIENT_HOOK);
			std::shared_ptr<Builder> withSpecular(const Colour4f& specular, const std::string& hook = DEFAULT_SPECULAR_HOOK);
			std::shared_ptr<Builder> withSpecularIntensity(const float& intensity, const std::string& hook= DEFAULT_INTENSITY_HOOK);

			Material build();

		private:
			Colour4f    diffuseColour;
			Colour4f    ambientColour;
			Colour4f    specularColour;
			float       intensity;
			GLuint      textureID;

			std::string diffuseShaderHook;
			std::string ambientShaderHook;
			std::string specularShaderHook;
			std::string intensityShaderHook;
			std::string textureSamplerHook;
			
			GLuint	AddTextureFromFile(const std::string& filename);
		};

		/*! \brief Create a material with default shader hooks assigned. */
		Material();

        /*! \returns The diffuse clour of the material. */
        Colour4f		GetDiffuseColour(void) const;
		/*! \returns The ambient clour of the material. */
		Colour4f		GetAmbientColour(void) const;
		/*! \returns The specular clour of the material. */
		Colour4f		GetSpecularColour(void) const;
		/*! \returns The intensity of the specular highlight. */
		float			GetIntensity(void) const;
		/*! \returns The texture ID of the material. */
		GLuint			GetTextureID(void) const;
        /*! \brief Applies this material to a shader.
         *
         * \param shader The shader to apply the material to.
         */
        virtual void	ApplyToShader(Shader& shader);
        /*! \brief Applies a texture, should be called before drawing a vertex object. */
        virtual void	ApplyTexture(Shader& shader, unsigned int unit = 0);

    private:
        GLenum                      GetTextureUnit(unsigned int unit);

        Colour4f    diffuseColour;
		Colour4f    ambientColour;
		Colour4f    specularColour;
		float       intensity;
		GLuint      textureID;

		std::string diffuseShaderHook;
		std::string ambientShaderHook;
		std::string specularShaderHook;
		std::string intensityShaderHook;
		std::string textureSamplerHook;
    };
}

#endif /* __ice_fairy_material_h__ */