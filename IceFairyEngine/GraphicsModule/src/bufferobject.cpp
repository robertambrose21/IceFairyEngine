#include "bufferobject.h"

using namespace IceFairy;

BufferObject::BufferObject()
    : type(DEPTH),
      width(0),
      height(0),
      framebufferTextureID(0),
      framebufferID(0)
{ }

BufferObject::BufferObject(std::string name, Type type, Shader shader, DrawFunction drawFunction, unsigned int width, unsigned int height)
    : name(name),
      type(type),
      shader(shader),
	  drawFunction(drawFunction),
      width(width),
      height(height),
      framebufferTextureID(0),
      framebufferID(0),
	  rboDepthStencilID(0)
{
    CreateBuffer();
}

BufferObject::~BufferObject() {
    if (framebufferTextureID != 0)
        glDeleteTextures(1, &framebufferTextureID);
    if (framebufferID != 0)
        glDeleteFramebuffers(1, &framebufferID);
	if (rboDepthStencilID != 0)
		glDeleteRenderbuffers(1, &rboDepthStencilID);
}

void BufferObject::CreateBuffer(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    glBindTexture(GL_TEXTURE_2D, 0);
    glGenTextures(1, &framebufferTextureID);
    glBindTexture(GL_TEXTURE_2D, framebufferTextureID);

    switch (type) {
        case DEPTH:
            CreateDepthBuffer();
            break;

        case COLOUR:
            CreateColourBuffer();
            break;

        default:
            throw BufferObjectException("Undefined type (must be either DEPTH or COLOUR): " + type);
    }

    CheckFrameBufferStatus();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BufferObject::CreateDepthBuffer(void) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebufferTextureID, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void BufferObject::CreateColourBuffer(void) {
    // TODO: change RGB8?
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebufferTextureID, 0);

	glGenRenderbuffers(1, &rboDepthStencilID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencilID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencilID);
}

void BufferObject::DrawInBuffer(unsigned int drawWidth, unsigned int drawHeight) {
    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawFunction(shader, shared_from_this());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (width != drawWidth || height != drawHeight)
        glViewport(0, 0, drawWidth, drawHeight);
}

void BufferObject::RecreateBuffer(unsigned int w, unsigned int h) {
    SetWidth(w);
    SetHeight(h);

    if (framebufferTextureID != 0)
        glDeleteTextures(1, &framebufferTextureID);
    if (framebufferID != 0)
        glDeleteFramebuffers(1, &framebufferID);
	if (rboDepthStencilID != 0)
		glDeleteRenderbuffers(1, &rboDepthStencilID);

    CreateBuffer();
}

void BufferObject::CheckFrameBufferStatus(void) {
    switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        case GL_FRAMEBUFFER_UNDEFINED:
            throw BufferObjectException("GL_FRAMEBUFFER_UNDEFINED");

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            throw BufferObjectException("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            throw BufferObjectException("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            throw BufferObjectException("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            throw BufferObjectException("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");

        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw BufferObjectException("GL_FRAMEBUFFER_UNSUPPORTED");

        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            throw BufferObjectException("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");

        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            throw BufferObjectException("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
    }
}

void BufferObject::SetWidth(const unsigned int& value) {
    this->width = value;
}

void BufferObject::SetHeight(const unsigned int& value) {
    this->height = value;
}

BufferObject::Type BufferObject::GetType(void) const {
    return type;
}

unsigned int BufferObject::GetWidth(void) const {
    return width;
}

unsigned int BufferObject::GetHeight(void) const {
    return height;
}

GLuint BufferObject::GetTextureID(void) const {
    return framebufferTextureID;
}

Shader BufferObject::GetShader(void) const {
    return shader;
}

std::string BufferObject::GetName(void) const {
    return name;
}