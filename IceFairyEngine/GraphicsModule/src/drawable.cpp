#include "drawable.h"

using namespace IceFairy;

Drawable::Drawable()
    : currentBufferIndex(0)
{ }

void Drawable::DrawBuffers(unsigned int width, unsigned int height) {
    for (auto& bufferObject : bufferObjects) {
		bufferObject->DrawInBuffer(width, height);
    }
}

unsigned int Drawable::AddBufferObject(std::shared_ptr<BufferObject> bufferObject) {
    bufferObjects.push_back(bufferObject);
    return currentBufferIndex++;
}

unsigned int Drawable::AddBufferObject(std::string name, BufferObject::Type type, Shader shader,
		unsigned int width, unsigned int height) {
	BufferObject::DrawFunction func = [&](Shader shader, std::shared_ptr<BufferObject> bo) {
		DrawFrame(shader, bo);
	};
    return AddBufferObject(std::make_shared<IceFairy::BufferObject>(name, type, shader, func, width, height));
}

std::shared_ptr<BufferObject> Drawable::GetBufferObject(unsigned int index) {
    return bufferObjects[index];
}

Drawable::BufferObjectList Drawable::GetBufferObjects(void) {
    return bufferObjects;
}