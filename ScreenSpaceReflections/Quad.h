#pragma once

#include "Renderable.h"

class Quad : public Renderable
{
private:
	GLuint fbo;
	GLuint rbo;
	GLuint depthTexture;
	GLuint depthTextureLoc;
	GLuint reflectionTexture;
	GLuint reflectionTextureLoc;
	GLuint posTexture;
	GLuint posTextureLoc;


protected:
	void initBuffers() override;
public:
	// between -1 and 1
	Quad(const string& vertName, const string& fragName, uint width, uint height, const std::vector<string>& cubemapFaces);
	GLuint getTextureLoc() { return colorTexture; }
	GLuint getFbo() { return fbo; }

	GLuint getDepthTexture() { return depthTexture; }
	GLuint getDepthTextureLoc() { return depthTextureLoc; }

	void bindColorTexture() { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, colorTexture); glUniform1i(colorTextureLoc, 0); }
	void bindDepthTexture() { glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, depthTexture); glUniform1i(depthTextureLoc, 1); }
	void bindNormalTexture() { glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, normalTexture); glUniform1i(normalTextureLoc, 2); }
	void bindReflectionTexture() { glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, reflectionTexture); glUniform1i(reflectionTextureLoc, 3); }
	void bindPosTexture() { glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, posTexture); glUniform1i(posTextureLoc, 4); }
	void bindCubeTex() { glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture); glUniform1i(cubeTexLoc, 5); }
};