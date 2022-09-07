#include "Skybox.h"

Skybox::Skybox(const std::vector<string>& faces, const string& vertName, const string& fragName)
{
	gProgram = Util::initShaders(vertName, fragName);

	cubeTexture = Util::loadCubemap(faces);

    viewMatLoc = glGetUniformLocation(gProgram, "viewMat");
    projMatLoc = glGetUniformLocation(gProgram, "projMat");
    modelMatLoc = glGetUniformLocation(gProgram, "modelMat");
    setShaderCubemapParamName("cubeTex");

    std::cout << "viewMatLoc->" << viewMatLoc << "\tprojMatLoc->" << projMatLoc << "\tcubemapLoc->" << cubeTexLoc << std::endl;

	gVertices.emplace_back(-1.0f, 1.0f, 1.0f);
	gVertices.emplace_back(-1.0f, -1.0f, 1.0f);
	gVertices.emplace_back(1.0f, -1.0f, 1.0f);
	gVertices.emplace_back(1.0f, 1.0f, 1.0f);
	
    
    gVertices.emplace_back(-1.0f, 1.0f, -1.0f);
	gVertices.emplace_back(-1.0f, -1.0f, -1.0f);
	gVertices.emplace_back(1.0f, -1.0f, -1.0f);
	gVertices.emplace_back(1.0f, 1.0f, -1.0f);

    // should start from bottom left vertex

	// right
	gFaces.emplace_back(7, 3, 2);
	gFaces.emplace_back(7, 2, 6);


    // left
    gFaces.emplace_back(4, 1, 0);
    gFaces.emplace_back(4, 5, 1);
    

    // top
    gFaces.emplace_back(3, 0, 4);
    gFaces.emplace_back(3, 4, 7);

    // bottom
    gFaces.emplace_back(2, 1, 5);
    gFaces.emplace_back(2, 5, 6);

    // back
    gFaces.emplace_back(4, 5, 6);
    gFaces.emplace_back(4, 6, 7);

    // front
    gFaces.emplace_back(1, 2, 3);
    gFaces.emplace_back(3, 0, 1);

    initBuffers_vertexOnly();

}

void Skybox::setShaderCubemapParamName(const string& name)
{
	cubeTexLoc = glGetUniformLocation(gProgram, name.c_str());
}


void Skybox::initBuffers()
{
    size_t gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    size_t indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);

    std::cout << gVertexDataSizeInBytes << std::endl;

    indexCount = gFaces.size() * 3;

    AvailableVao++;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    std::cout << "vao = " << vao << std::endl;

    glEnableVertexAttribArray(0);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, &gVertices[0]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, &gFaces[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)96);
}