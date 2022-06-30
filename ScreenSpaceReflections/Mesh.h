#pragma once

#include "Renderable.h"

class Mesh : public Renderable
{
public:
	Mesh(const string& objName, const string& vert, const string& frag);
};