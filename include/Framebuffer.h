#pragma once
#include <vector>
#include <glad/glad.h>
#include "TextureManager.h"
#include <iostream>

struct Attachment {
	unsigned int id = 0;
	GLenum internalFormat = 0;
	GLenum format = 0;
	GLenum type = 0;
	bool isTexture = true;
};

enum class AttachmentType {
	Color,
	Depth,
	Stencil,
	DepthStencil,
	None
};

class Framebuffer {

public:
	unsigned int id;
	unsigned int width, height;
	std::vector<Attachment> color;
	Attachment depthStencil;
	AttachmentType depthstencilType = AttachmentType::None;

	Framebuffer(unsigned int width, unsigned int height)
		: width(width), height(height) {
		glGenFramebuffers(1, &id);
	}
	void create() {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		for(int i = 0; i < color.size(); ++i)
			processAttachments(color[i], GL_COLOR_ATTACHMENT0, i);
		if(depthstencilType == AttachmentType::Depth)
			processAttachments(depthStencil, GL_DEPTH_ATTACHMENT);
		if(depthstencilType == AttachmentType::Stencil)
			processAttachments(depthStencil, GL_STENCIL_ATTACHMENT);
		if(depthstencilType == AttachmentType::DepthStencil)
			processAttachments(depthStencil, GL_DEPTH_STENCIL_ATTACHMENT);

		if (color.empty()) {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void attachEmptyTexture(AttachmentType attType, GLenum internalFormat, GLenum format, GLenum type = GL_UNSIGNED_BYTE) {
		Attachment att = { 0, internalFormat, format, type, true };
		pushAttachment(attType, att);
	}
	void attachEmptyRenderbuffer(AttachmentType attType, GLenum internalFormat, GLenum type = GL_UNSIGNED_BYTE) {
		Attachment att = { 0, internalFormat, 0, type, false };
		pushAttachment(attType, att);
	}
	void attachTexture(AttachmentType attType, unsigned int textureId) {
		Attachment att = { textureId };
		pushAttachment(attType, att);
	}
	void attachRenderbuffer(AttachmentType attType, unsigned int renderbufferId) {
		Attachment att = { renderbufferId };
		pushAttachment(attType, att);
	}

	unsigned int getID(AttachmentType type, unsigned int index = 0) {
		if (type == AttachmentType::Color) {
			return color[index].id;
		}
		else if (type == AttachmentType::Depth || 
				 type == AttachmentType::Stencil ||
				 type == AttachmentType::DepthStencil) {
			return depthStencil.id;
		}
		return -1;
	}

	void clear() {
		for (auto& att : color) {
			if (!att.isTexture && att.id != 0) {
				glDeleteRenderbuffers(1, &att.id);
			}
		}
		if (!depthStencil.isTexture && depthStencil.id != 0) {
			glDeleteRenderbuffers(1, &depthStencil.id);
		}
		color.clear();
		depthstencilType = AttachmentType::None;
	}

	void bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glViewport(0, 0, width, height);
	}

	void unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~Framebuffer() {
		clear();
		glDeleteFramebuffers(1, &id);
	}

private:

	void processAttachments(Attachment& att, GLenum attachmentType, unsigned int i = 0) {
		if (att.id == 0) {
			if (att.isTexture == true) {
				//create texture
				att.id = TextureManager::Get().createTexture(width, height, att.internalFormat, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			}
			else {
				//create renderbuffer
				glGenRenderbuffers(1, &att.id);
				glBindRenderbuffer(GL_RENDERBUFFER, att.id);
				glRenderbufferStorage(GL_RENDERBUFFER, att.internalFormat, width, height);
			}
		}
		if (att.isTexture) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType + i, GL_TEXTURE_2D, att.id, 0);
		}
		else {
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType + i, GL_RENDERBUFFER, att.id);
		}
	}

	void pushAttachment(AttachmentType attType, const Attachment& att) {
		if (attType == AttachmentType::Color) {
			color.push_back(att);
		}
		else {
			if (depthstencilType != AttachmentType::None) {
				std::cerr << "WARNING::FRAMEBUFFER:: Depth/Stencil slot is already occupied! Ignoring attachment." << std::endl;
			}
			depthstencilType = attType;
			depthStencil = att;
		}
	}
};
