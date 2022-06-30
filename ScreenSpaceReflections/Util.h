#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
//#include <OpenGL/gl3.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include "glm/glm.hpp" // GL Math library header
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"

#define string std::string

#define vec2 glm::vec2
#define vec3 glm::vec3
#define vec4 glm::vec4

#define int3 glm::int3

#define mat3 glm::mat3
#define mat4 glm::mat4

#define uint glm::uint

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

 struct Face
{
    Face() {}
    Face(int v[], int t[], int n[]) {
        vIndex[0] = v[0];
        vIndex[1] = v[1];
        vIndex[2] = v[2];
        tIndex[0] = t[0];
        tIndex[1] = t[1];
        tIndex[2] = t[2];
        nIndex[0] = n[0];
        nIndex[1] = n[1];
        nIndex[2] = n[2];
    }
    GLuint vIndex[3], tIndex[3], nIndex[3];


};

 enum ShaderType
 {
     VERT,
     FRAG
 };

class Util
{
public:
    static GLuint loadTexture(const string& name);
    static GLuint loadCubemap(const std::vector<string>& faces);
    static GLuint emptyCubemap(int size);
    static bool parseObj(const string& name, 
        std::vector<vec3>& gVertices, 
        std::vector<vec3>& gNormals, 
        std::vector<vec2>& gTexCoords, 
        std::vector<int3>& gFaces);

    static GLuint initShaders(const string& vertName, const string& fragName);

    static bool readShader(const string& name, string& data);

};