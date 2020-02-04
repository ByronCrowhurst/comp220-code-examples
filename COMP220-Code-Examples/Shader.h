#pragma once
#include <iostream>
#include <fstream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <sstream>
#include <string>
#include <vector>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);