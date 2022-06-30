#include "Renderable.h"

GLuint Renderable::AvailableVao = 0;
GLuint Renderable::ActiveProgram = 0;

void Renderable::setShaderParamNames(const string& modelMatName = "modelMat", const string& viewMatName = "viewMat", const string& projMatName = "projMat")
{
	modelMatLoc = glGetUniformLocation(gProgram, modelMatName.c_str());
	viewMatLoc = glGetUniformLocation(gProgram, viewMatName.c_str());
	projMatLoc = glGetUniformLocation(gProgram, projMatName.c_str());

    std::cout << name << std::endl;
    std::cout << "modelMatLoc->" << modelMatLoc << std::endl;
    std::cout << "viewMatLoc->" << viewMatLoc << std::endl;
    std::cout << "projMatLoc->" << projMatLoc << std::endl;
}

void Renderable::setModelMat(const mat4& modelMat)
{
    this->modelMat = modelMat;
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
}

void Renderable::setViewMat(const mat4& viewMat)
{
    this->viewMat = viewMat;
    glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
}
void Renderable::setProjMat(const mat4& projMat)
{
    this->projMat = projMat;
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));
}

void Renderable::generateEmptyCubemap()
{

}


void Renderable::initBuffers()
{
    size_t gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    size_t gNormalDataSizeInBytes = gNormals.size() * 3 * sizeof(GLfloat);
    size_t gTexCoordDataSizeInBytes = gTexCoords.size() * 2 * sizeof(GLfloat);
    size_t indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);

    std::cout << gVertexDataSizeInBytes << std::endl;
    std::cout << gNormalDataSizeInBytes << std::endl;
    std::cout << gTexCoordDataSizeInBytes << std::endl;

    indexCount = gFaces.size() * 3;

    AvailableVao++;
    std::cout << "AVAILABLE_VAO-->" << AvailableVao << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    std::cout << "vao = " << vao << std::endl;

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes
        + gNormalDataSizeInBytes
        + gTexCoordDataSizeInBytes,
        0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, &gVertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, &gNormals[0]);
    if (gTexCoordDataSizeInBytes > 0)
        glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, gTexCoordDataSizeInBytes, &gTexCoords[0]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, &gFaces[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes + gNormalDataSizeInBytes));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)96);
}

void Renderable::initBuffers_vertexOnly()
{
    size_t gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    size_t indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);

    std::cout << gVertexDataSizeInBytes << std::endl;

    indexCount = gFaces.size() * 3;

    AvailableVao++;

    glGenVertexArrays(AvailableVao, &vao);
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