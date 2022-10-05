#pragma once

#define GLFW_DLL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <string>
#include <iostream>

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

#include "event.h"
#include "window.h"
#include "./utils/geometries/mesh.h"
#include "./utils/geometries/sphere.h"
#include "light.h"
#include "material.h"
#include "scene.h"
#include "layer.h"
#include "camera.h"
#include "layerstack.h"
#include "GUIManager.h"
#include "renderer.h"
#include "./utils/shaderProgram.h"
#include "./utils/textureLoader.h"
#include "application.h"

