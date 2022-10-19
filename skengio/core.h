#pragma once

#ifndef _SK_CORE_
#define _SK_CORE_

#define GLEW_STATIC
#define GLFW_DLL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <string>
#include <iostream>
#include "audiosource.h"
#include "event.h"
#include "window.h"
#include "skengio/utils/geometries/mesh.h"
#include "skengio/utils/geometries/sphere.h"
#include "light.h"
#include "material.h"
#include "scene.h"
#include "layer.h"
#include "camera.h"
#include "layerstack.h"
#include "GUIManager.h"
#include "renderer.h"
#include "skengio/utils/shaderProgram.h"
#include "skengio/utils/textureLoader.h"
#include "application.h"

#endif