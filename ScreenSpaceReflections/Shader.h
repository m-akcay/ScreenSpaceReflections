#pragma once

#include "Util.h"

class Shader
{
private:
	GLuint program;
public:
	Shader(const string& name, ShaderType stype, GLuint gProgram);
};