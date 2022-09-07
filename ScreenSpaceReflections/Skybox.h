#pragma once

#include "Renderable.h"

class Skybox : public Renderable
{
protected:
	void initBuffers() override;
public:
	Skybox(const std::vector<string>& faces, const string& vertName, const string& fragName);
	void setShaderCubemapParamName(const string& name);

	GLuint getCubemap() { return cubeTexture; }
};