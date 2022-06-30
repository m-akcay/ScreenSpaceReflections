#include "Util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//static void Util::loadTexture;

GLuint Util::loadTexture(const string& name)
{
    unsigned int texture;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

GLuint Util::loadCubemap(const std::vector<string>& faces)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        //stbi_set_flip_horizontally_on_load(true);
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // nrChannels is assumed to be 3
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            exit(-1);
        }
    } 

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

GLuint Util::emptyCubemap(int size)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);


    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, 0
            );
    }

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

bool Util::parseObj(const string& name, 
    std::vector<vec3>& gVertices, 
    std::vector<vec3>& gNormals, 
    std::vector<vec2>& gTexCoords, 
    std::vector<int3>& gFaces)
{
    std::fstream myfile;

    // Open the input 
    myfile.open(name.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            std::stringstream str(curLine);
            GLfloat c1, c2, c3;
            GLuint index[9];
            string tmp;

            if (curLine.length() >= 2)
            {
                if (curLine[0] == 'v')
                {
                    if (curLine[1] == 't') // texture
                    {
                        str >> tmp; // consume "vt"
                        str >> c1 >> c2;
                        gTexCoords.push_back(vec2(c1, c2));
                    }
                    else if (curLine[1] == 'n') // normal
                    {
                        str >> tmp; // consume "vn"
                        str >> c1 >> c2 >> c3;
                        gNormals.push_back(vec3(c1, c2, c3));
                    }
                    else // vertex
                    {
                        str >> tmp; // consume "v"
                        str >> c1 >> c2 >> c3;
                        gVertices.push_back(vec3(c1, c2, c3));
                    }
                }
                else if (curLine[0] == 'f') // face
                {
                    str >> tmp; // consume "f"
                    char c;
                    int vIndex[3], nIndex[3], tIndex[3];
                    str >> vIndex[0]; str >> c >> c; // consume "//"
                    str >> nIndex[0];
                    str >> vIndex[1]; str >> c >> c; // consume "//"
                    str >> nIndex[1];
                    str >> vIndex[2]; str >> c >> c; // consume "//"
                    str >> nIndex[2];

                    assert(vIndex[0] == nIndex[0] &&
                        vIndex[1] == nIndex[1] &&
                        vIndex[2] == nIndex[2]); // a limitation for now

                 // make indices start from 0
                    for (int c = 0; c < 3; ++c)
                    {
                        vIndex[c] -= 1;
                        nIndex[c] -= 1;
                        tIndex[c] -= 1;
                    }

                    gFaces.push_back(int3(vIndex[0], vIndex[1], vIndex[2]));
                }
                else
                {
                    std::cout << "Ignoring unidentified line in obj file: " << curLine << std::endl;
                }
            }

        }

        myfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool Util::readShader(const string& name, string& data)
{
    std::fstream myfile;

    // Open the input 
    myfile.open(name.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            data += curLine;
            if (!myfile.eof())
            {
                data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

GLuint Util::initShaders(const string& vertName, const string& fragName)
{
    string vertData, fragData;
    GLuint gProgram = glCreateProgram();
    
    if (!readShader(vertName, vertData))
    {
        std::cout << "Cannot find file name: " + vertName << std::endl;
        exit(-1);
    }
    else if (!readShader(fragName, fragData))
    {
        std::cout << "Cannot find file name: " + fragName << std::endl;
        exit(-1);
    }

    GLint length = vertData.length();
    auto shaderData = (const GLchar*)vertData.c_str();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shaderData, &length);
    glCompileShader(vs);

    // ???? about shaderData
    length = fragData.length();
    shaderData = (const GLchar*)fragData.c_str();
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shaderData, &length);
    glCompileShader(fs);

    glAttachShader(gProgram, vs);
    glAttachShader(gProgram, fs);


    // logs and status
    char output[2][1024] = { 0 };
    glGetShaderInfoLog(vs, 1024, &length, output[0]);
    glGetShaderInfoLog(fs, 1024, &length, output[1]);
    printf("VS compile log: %s\n", output[0]);
    printf("FS compile log: %s\n", output[1]);

    GLint status;

    glLinkProgram(gProgram);
    glGetProgramiv(gProgram, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        std::cout << "Program link failed" << std::endl;
        exit(-1);
    }

    return gProgram;
}