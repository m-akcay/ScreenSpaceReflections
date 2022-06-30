#include "Mesh.h"

Mesh::Mesh(const string& objName, const string& vertName, const string& fragName)
{
	this->name = objName;
	Util::parseObj(objName, gVertices, gNormals, gTexCoords, gFaces);
	gProgram = Util::initShaders(vertName, fragName);
	setShaderParamNames("modelMat", "viewMat", "projMat");
	initBuffers();
}
