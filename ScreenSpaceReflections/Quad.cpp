#include "Quad.h"

Quad::Quad(const string& vertName, const string& fragName, uint width, uint height, const std::vector<string>& cubemapFaces)
{
    
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &posTexture);
    glBindTexture(GL_TEXTURE_2D, posTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, reflectionTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, posTexture, 0);
    
    cubeTexture = Util::loadCubemap(std::vector<string>{"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"});
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        exit(-1);
    }
    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    gVertices.emplace_back(-1, -1, 0);
	gVertices.emplace_back(1, -1, 0);
	gVertices.emplace_back(1, 1, 0);
	gVertices.emplace_back(-1, 1, 0);

    gTexCoords.emplace_back(0.0f, 0.0f);
    gTexCoords.emplace_back(1.0f, 0.0f);
    gTexCoords.emplace_back(1.0f, 1.0f);
    gTexCoords.emplace_back(0.0f, 1.0f);

	gFaces.emplace_back(0, 1, 2);
	gFaces.emplace_back(0, 2, 3);

    gProgram = Util::initShaders(vertName, fragName);
    
    colorTextureLoc = glGetUniformLocation(gProgram, "colorTexture");
    depthTextureLoc = glGetUniformLocation(gProgram, "depthTexture");
    normalTextureLoc = glGetUniformLocation(gProgram, "normalTexture");
    reflectionTextureLoc = glGetUniformLocation(gProgram, "reflectionTexture");
    posTextureLoc = glGetUniformLocation(gProgram, "posTexture");
    cubeTexLoc = glGetUniformLocation(gProgram, "cubeTexture");
    
    std::cout << "HELO_colorTex->" << colorTexture << "   " << "HELO_colorTexLoc->" << colorTextureLoc << std::endl;
    std::cout << "HELO_depthTex->" << depthTexture << "   " << "HELO_depthTexLoc->" << depthTextureLoc << std::endl;
    std::cout << "HELO_normalTex->" << normalTexture << "   " << "HELO_normalTexLoc->" << normalTextureLoc << std::endl;
    std::cout << "HELO_cubeTex->" << cubeTexture << "   " << "HELO_cubeTexLoc->" << cubeTexLoc << std::endl;
	initBuffers();
	
}

void Quad::initBuffers()
{
    size_t gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    size_t gTexCoordDataSizeInBytes = gTexCoords.size() * 2 * sizeof(GLfloat);
    size_t indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);

    std::cout << gVertexDataSizeInBytes << std::endl;
    std::cout << gTexCoordDataSizeInBytes << std::endl;

    indexCount = gFaces.size() * 3;

    AvailableVao++;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    std::cout << "vao = " << vao << std::endl;

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gTexCoordDataSizeInBytes,
        0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, &gVertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gTexCoordDataSizeInBytes, &gTexCoords[0]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, &gFaces[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)96);
}