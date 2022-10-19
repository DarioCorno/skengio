#pragma once
#ifndef _SK_OBJECT_
#define _SK_OBJECT_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <skengio/material.h>
#include <skengio/renderParams.h>
#include "shaderProgram.h"
#include "geometries/mesh.h"

namespace SKEngio {


	class Object {
		public:

			Object() { 
				model = glm::mat4(1.0f);
				cubemap = NULL;
				material = new SKEngio::Material();
				castsShadows = true;
			}

			~Object() {
				delete mesh;
				delete shader;
				delete material;
			}

			void render(RenderParams* rp) {

				if ( (rp->pass == RenderPass::ShadowDepth) && (!castsShadows) )
					return;

				shader->bind();
				material->diffuseTexture->bind();

				shader->SetDiffTexture(material->diffuseTexture->textureUnit);

				if (cubemap != NULL) {
					cubemap->bind();
					shader->SetCubeTexture(cubemap->textureUnit);
				}

				shader->SetModelUniforms(model);
				mesh->draw();

				material->diffuseTexture->unbind();
			}

			void resetTransforms() {
				model = glm::mat4(1.0f);
			}

			void translate(float x, float y, float z) {
				model = glm::translate(model, glm::vec3(x, y, z));
			}

			void rotate(float angle, float x, float y, float z) {
				model = glm::rotate(model, angle, glm::vec3(x, y, z));
			}

			void scale(float x, float y, float z) {
				model = glm::scale( model, glm::vec3(x, y, z) );
			}

			void setCubemap(Texture* _cubemap) {
				cubemap = _cubemap;
			}

			Mesh* mesh;
			ShaderProgram* shader;
			glm::mat4 model;
			Material* material;
			bool castsShadows;

		private:
			Texture* cubemap;

	};
}

#endif