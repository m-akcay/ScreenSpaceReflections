#pragma once

#include "Util.h"

class Renderable
{
protected:
	string name;
	static GLuint ActiveProgram;
	static GLuint AvailableVao;

	GLuint gProgram;
	GLuint vao;
	GLuint gVertexAttribBuffer;
	GLuint gIndexBuffer;
	GLuint indexCount;

	GLuint modelMatLoc;
	GLuint viewMatLoc;
	GLuint projMatLoc;
	GLuint cubeTexLoc;
	GLuint colorTextureLoc;
	GLuint normalTextureLoc;
	
	mat4 modelMat;
	mat4 viewMat;
	mat4 projMat;
	GLuint cubeTexture;
	GLuint colorTexture;
	GLuint normalTexture;

	std::vector<vec3> gVertices;
	std::vector<vec2> gTexCoords;
	std::vector<vec3> gNormals;
	std::vector<int3> gFaces;
	
	Renderable() {}

	virtual void initBuffers();
	virtual void initBuffers_vertexOnly();
public:
	void setModelMat(const mat4& modelMat);
	void setViewMat(const mat4& viewMat);
	void setProjMat(const mat4& projMat);
	void setShaderParamNames(const string& modelMatName, const string& viewMatName, const string& projMatName);
	inline void setShaderFloat(const string& paramName, float val);

	void generateEmptyCubemap();
	inline void activateProgram();

	GLuint getProgram() { return gProgram; }
	GLuint getVao() { return vao; }
	GLuint getVertexAttribBuffer() { return gVertexAttribBuffer; }
	GLuint getIndexBuffer() { return gIndexBuffer; }
	GLuint getIndexCount() { return indexCount; }
	GLuint getColorTextureLoc() { return colorTexture; }


	std::vector<vec3>& getVertices() { return gVertices; }
	std::vector<vec2>& getTexCoords() { return gTexCoords;}
	std::vector<vec3>& getNormals()	 { return gNormals;}
	std::vector<int3>& getFaces()	{ return gFaces;}
};

void Renderable::activateProgram()
{
	if (gProgram != ActiveProgram)
	{
		ActiveProgram = gProgram;
		glUseProgram(gProgram);
	}
}

void Renderable::setShaderFloat(const string& paramName, float val)
{
	activateProgram();
	GLuint loc = glGetUniformLocation(gProgram, paramName.c_str());
	glUniform1f(loc, val);
}