#ifndef __ice_fairy_point_light_h__
#define __ice_fairy_point_light_h__

#define ICE_FAIRY_MAX_POINT_LIGHTS 50

#include <string>

#include "../shader.h"
#include "../../math/colour.h"
#include "../../math/vector.h"
#include "../../core/utilities/icexception.h"

namespace IceFairy {
    /*! \brief Thrown if the maximum number of possible active point lights has been exceeded. */
    class MaxPointLightsExceededException : public ICException {
    public:
        /*! \internal */
        MaxPointLightsExceededException()
            : ICException("The maximum number of Point Lights has been exceeded (Maximum of " +
                std::to_string(ICE_FAIRY_MAX_POINT_LIGHTS) + ").")
        { }
    };

    /*! \brief TODO: doc */
    class PointLight {
    public:
        PointLight() { }
        PointLight(Vector3f position, Colour3f colour, float ambient,
            float cAttenuation, float lAttenuation = 0.0f, float eAttenuation = 0.0f);

        Vector3f    GetPosition(void) const;
        void        SetPosition(const Vector3f& position);
        Colour3f    GetColour(void) const;
        void        SetColour(const Colour3f& colour);
        float       GetAmbient(void) const;
        void        SetAmbient(const float& ambient);
        float       GetConstantAttentuation(void) const;
        void        SetConstantAttentuation(const float& cAttenuation);
        float       GetLinearAttentuation(void) const;
        void        SetLinearAttentuation(const float& lAttenuation);
        float       GetExponentialAttentuation(void) const;
        void        SetExponentialAttentuation(const float& eAttenuation);

        void        Draw(Shader shader, const unsigned int& index, const Vector3f& offset);

    private:
        std::string GetPointLightShaderName(const unsigned int& index, std::string append);

        Vector3f    position;
        Colour3f    colour;
        float       ambient;
        float       cAttenuation;
        float       lAttenuation;
        float       eAttenuation;
    };
}

#endif /* __ice_fairy_point_light_h__ */