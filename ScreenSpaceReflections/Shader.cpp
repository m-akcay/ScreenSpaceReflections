#include "Shader.h"

Shader::Shader(const string& name, ShaderType stype, GLuint gProgram)
{
    string data;

    if (!Util::readShader(name, data))
    {
        std::cout << "Cannot find file name: " + name << std::endl;
        exit(-1);
    }

    GLint length = data.length();
    auto shaderData = (const GLchar*)data.c_str();
    GLuint vs;

    if (stype == ShaderType::VERT)
    {
        vs = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (stype == ShaderType::FRAG)
    {

    }
    else
    { 
        std::cout << "unidentified shader type" << std::endl;
        exit(-1);
    }

    glShaderSource(vs, 1, &shaderData, &length);
    glCompileShader(vs);
    glAttachShader(gProgram, vs);


    // logs and status
    char output[1024] = { 0 };
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

    GLint status;

    glLinkProgram(gProgram);
    glGetProgramiv(gProgram, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        std::cout << "Program link failed" << std::endl;
        exit(-1);
    }
}