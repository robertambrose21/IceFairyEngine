#include "pointlight.h"

using namespace IceFairy;

PointLight::PointLight(Vector3f position, Colour3f colour, float ambient,
    float cAttenuation, float lAttenuation, float eAttenuation)
    : position(position),
      colour(colour),
      ambient(ambient),
      cAttenuation(cAttenuation),
      lAttenuation(lAttenuation),
      eAttenuation(eAttenuation)
{ }

Vector3f PointLight::GetPosition(void) const {
    return position;
}

void PointLight::SetPosition(const Vector3f& position) {
    this->position = position;
}

Colour3f PointLight::GetColour(void) const {
    return colour;
}

void PointLight::SetColour(const Colour3f& colour) {
    this->colour = colour;
}

float PointLight::GetAmbient(void) const {
    return ambient;
}

void PointLight::SetAmbient(const float& ambient) {
    this->ambient = ambient;
}

float PointLight::GetConstantAttentuation(void) const {
    return cAttenuation;
}

void PointLight::SetConstantAttentuation(const float& cAttenuation) {
    this->cAttenuation = cAttenuation;
}

float PointLight::GetLinearAttentuation(void) const {
    return lAttenuation;
}

void PointLight::SetLinearAttentuation(const float& lAttenuation) {
    this->lAttenuation = lAttenuation;
}

float PointLight::GetExponentialAttentuation(void) const {
    return eAttenuation;
}

void PointLight::SetExponentialAttentuation(const float& eAttenuation) {
    this->eAttenuation = eAttenuation;
}

void PointLight::Draw(Shader shader, const unsigned int& index, const Vector3f& offset) {
    shader.SetUniform3f(GetPointLightShaderName(index, "colour"), GetColour());
    shader.SetUniform3f(GetPointLightShaderName(index, "position"), GetPosition() + offset);
    shader.SetUniform1f(GetPointLightShaderName(index, "ambient"), GetAmbient());
    shader.SetUniform1f(GetPointLightShaderName(index, "constantAtt"), GetConstantAttentuation());
    shader.SetUniform1f(GetPointLightShaderName(index, "linearAtt"), GetLinearAttentuation());
    shader.SetUniform1f(GetPointLightShaderName(index, "exponentialAtt"), GetExponentialAttentuation());
}

std::string PointLight::GetPointLightShaderName(const unsigned int& index, std::string append) {
    return "pointLights[" + std::to_string(index) + "]." + append;
}