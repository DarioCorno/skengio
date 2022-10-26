#pragma once
#ifndef SK_ENTITY_
#define SK_ENTITY_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <skengio/material.h>
#include <skengio/renderParams.h>
#include <skengio/utils/shaderProgram.h>
#include "geometries/mesh.h"
#include "skengio/utils/transform.h"

namespace SKEngio {

	class Entity {
		public:

			Entity() : 
				material {new Material()},
				castsShadows {true}
		    {
			}

			~Entity() {
				delete mesh;
				delete shader;
				delete material;
				delete cubemap;
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

				shader->SetModelUniforms( transform.getModelMatrix() );
				mesh->draw();

				if (material->diffuseTexture)
					material->diffuseTexture->unbind();

			}

			void resetTransforms() {
				transform.resetTransforms();
			}

			void translate(float x, float y, float z) {
				transform.Translate( glm::vec3(x, y, z));
			}

			void rotate(float angle, float x, float y, float z) {
				transform.Rotate( angle, glm::vec3(x, y, z));
			}

			void scale(float x, float y, float z) {
				transform.Scale(  glm::vec3(x, y, z) );
			}

			void setCubemap(Texture* _cubemap) {
				cubemap = _cubemap;
			}

			void setParent(Entity* _parent) {
				parent = _parent;
			}

			template<typename... TArgs>
			void addChild(TArgs&... args) {
				childs.push_back(args...);
				childs.back()->setParent( this );
			}

			Mesh* mesh{};
			ShaderProgram* shader{};
			Transform transform;
			Material* material;
			bool castsShadows;

		private:

			Entity* parent;
			std::vector<Entity*> childs;
			Texture* cubemap{};

	};
}

#endif