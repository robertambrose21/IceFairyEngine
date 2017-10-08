#include "material.h"

using namespace IceFairy;

const std::string Material::DEFAULT_DIFFUSE_HOOK = "diffuseColour";
const std::string Material::DEFAULT_SAMPLER_HOOK = "texSampler";
const std::string Material::DEFAULT_AMBIENT_HOOK = "ambientColour";
const std::string Material::DEFAULT_SPECULAR_HOOK = "specularColour";
const std::string Material::DEFAULT_INTENSITY_HOOK = "intensity";

//const Material Material::DIFFUSE_RED = Material(Colour4f(1.0f, 0.0f, 0.0f, 1.0f));
//const Material Material::DIFFUSE_GREEN = Material(Colour4f(0.0f, 1.0f, 0.0f, 1.0f));
//const Material Material::DIFFUSE_BLUE = Material(Colour4f(0.0f, 0.0f, 1.0f, 1.0f));
//const Material Material::DIFFUSE_YELLOW = Material(Colour4f(1.0f, 1.0f, 0.0f, 1.0f));
//const Material Material::DIFFUSE_PURPLE = Material(Colour4f(1.0f, 0.0f, 1.0f, 1.0f));
//const Material Material::DIFFUSE_TEAL = Material(Colour4f(0.0f, 1.0f, 1.0f, 1.0f));
//const Material Material::DIFFUSE_BLACK = Material(Colour4f(0.0f, 0.0f, 0.0f, 1.0f));
//const Material Material::DIFFUSE_WHITE = Material(Colour4f(1.0f, 1.0f, 1.0f, 1.0f));

Material::Material()
    : textureID(0)
{ }

Colour4f Material::GetDiffuseColour(void) const {
    return diffuseColour;
}

Colour4f Material::GetAmbientColour(void) const {
	return ambientColour;
}

Colour4f Material::GetSpecularColour(void) const {
	return specularColour;
}

float Material::GetIntensity(void) const {
	return intensity;
}

GLuint IceFairy::Material::GetTextureID(void) const {
	return textureID;
}

void Material::ApplyToShader(Shader& shader) {
    shader.SetUniform4f(diffuseShaderHook.c_str(), diffuseColour);
}

void Material::ApplyTexture(Shader& shader, unsigned int unit) {
    if (textureID == 0)
        return;

    GLenum activeTexture = GetTextureUnit(unit);
    shader.SetUniform1i(textureSamplerHook.c_str(), unit);
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLenum Material::GetTextureUnit(unsigned int unit) {
    GLenum u = unit + GL_TEXTURE0;
    
    if (u > GL_TEXTURE31)
        throw InvalidTextureUnitException();
    else
        return u;
}

Material::Builder::Builder()
	: diffuseShaderHook(DEFAULT_DIFFUSE_HOOK),
	  textureSamplerHook(DEFAULT_SAMPLER_HOOK),
	  ambientShaderHook(DEFAULT_AMBIENT_HOOK),
	  specularShaderHook(DEFAULT_SPECULAR_HOOK),
	  intensityShaderHook(DEFAULT_INTENSITY_HOOK),
	  textureID(0)
{ }

std::shared_ptr<Material::Builder> Material::Builder::withDiffuse(const Colour4f& diffuse, const std::string& hook) {
	this->diffuseColour = diffuse;
	this->diffuseShaderHook = hook;
	return shared_from_this();
}

std::shared_ptr<Material::Builder> Material::Builder::withTexture(const std::string& filename, const std::string& hook) {
	this->textureID = AddTextureFromFile(filename);
	this->textureSamplerHook = hook;
	return shared_from_this();
}

std::shared_ptr<Material::Builder> Material::Builder::withAmbient(const Colour4f& ambient, const std::string& hook) {
	this->ambientColour = ambient;
	this->ambientShaderHook = hook;
	return shared_from_this();
}

std::shared_ptr<Material::Builder> Material::Builder::withSpecular(const Colour4f& specular, const std::string& hook) {
	this->specularColour = specular;
	this->specularShaderHook = hook;
	return shared_from_this();
}

std::shared_ptr<Material::Builder> Material::Builder::withSpecularIntensity(const float& intensity, const std::string& hook) {
	this->intensity = intensity;
	this->intensityShaderHook = hook;
	return shared_from_this();
}

Material Material::Builder::build() {
	Material m;
	m.diffuseColour = this->diffuseColour;
	m.ambientColour = this->ambientColour;
	m.specularColour = this->specularColour;
	m.intensity = this->intensity;
	m.textureID = this->textureID;
	m.diffuseShaderHook = this->diffuseShaderHook;
	m.ambientShaderHook = this->ambientShaderHook;
	m.specularShaderHook = this->specularShaderHook;
	m.intensityShaderHook = this->intensityShaderHook;
	m.textureSamplerHook = this->textureSamplerHook;

	return m;
}

GLuint Material::Builder::AddTextureFromFile(const std::string& filename) {
	Resource file(filename);
	int len = file.GetFileLength();
	char* buffer = new char[len + 1];
	file.GetData(buffer, len);
	GLuint texture;

	int x, y, n;
	unsigned char* data = stbi_load_from_memory((stbi_uc*)buffer, len, &x, &y, &n, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	delete[] buffer;

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}