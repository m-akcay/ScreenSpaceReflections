#pragma once

#include "Renderable.h"

class Quad : public Renderable
{
private:
	GLuint fbo;
	GLuint rbo;
	GLuint depthTexture;
	GLuint depthTextureLoc;


protected:
	void initBuffers() override;
public:
	// between -1 and 1
	Quad(const string& vertName, const string& fragName, uint width, uint height);
	GLuint getTextureLoc() { return colorTexture; }
	GLuint getFbo() { return fbo; }

	GLuint getDepthTexture() { return depthTexture; }
	GLuint getDepthTextureLoc() { return depthTextureLoc; }

	void bindDepthTexture() { glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, depthTexture); glUniform1i(depthTextureLoc, 1); }
	void bindColorTexture() { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, colorTexture); glUniform1i(colorTextureLoc, 0); }
};