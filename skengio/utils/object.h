#pragma once
#ifndef SK_OBJECT_
#define SK_OBJECT_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <skengio/material.h>
#include <skengio/renderParams.h>
#include "shaderProgram.h"
#include "geometries/mesh.h"

namespace SKEngio {


	class Object {
		public:

			Object() : 
				model {glm::mat4(1.0f)},
				material {new Material()},
				castsShadows {true}
		    {
			}

			~Object() {
				delete mesh;
				delete shader;
				delete material;
				delete cubemap;
			}

			void basicRender() {
				shader->bind();
				shader->SetModelUniforms(model);
				mesh->draw();
			}

			void render(RenderParams* rp) {

				if ( (rp->pass == RenderPass::ShadowDepth) && (!castsShadows) )
					return;

				shader->bind();

				if (material->diffuseTexture) {
					material->diffuseTexture->bind();
					shader->SetDiffTexture(material->diffuseTexture->textureUnit);
				}

				if (cubemap != nullptr) {
					cubemap->bind();
					shader->SetCubeTexture(cubemap->textureUnit);
				}

				shader->SetModelUniforms(model);
				mesh->draw();

				if (material->diffuseTexture)
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

			Mesh* mesh{};
			ShaderProgram* shader{};
			glm::mat4 model;
			Material* material;
			bool castsShadows;

		private:
			Texture* cubemap{};

	};
}

#endif