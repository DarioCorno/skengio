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


			//Update transform if it was changed
			void updateSelfAndChild()
			{
				if (!transform.isDirty())
					return;

				forceUpdateSelfAndChild();
			}

			//Force update of transform even if local space don't change
			void forceUpdateSelfAndChild()
			{
				if (parent)
					transform.computeModelMatrix(parent->transform.getModelMatrix());
				else
					transform.computeModelMatrix();

				for (auto&& child : childs)
				{
					child->forceUpdateSelfAndChild();
				}
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

				shader->SetCameraUniforms(rp->camera);
				shader->SetModelUniforms( transform.getModelMatrix() );
				mesh->draw();

				if (material->diffuseTexture)
					material->diffuseTexture->unbind();

				renderChilds(rp);

			}

			void renderChilds(RenderParams* rp) {
				for (Entity* child : childs) {
					child->render(rp);
				}
			}

			void resetTransforms() {
				transform.resetTransforms();
			}

			void translate(float x, float y, float z) {
				transform.setLocalPosition( glm::vec3(x, y, z));
			}

			void rotate(float angle, float x, float y, float z) {
				transform.setLocalRotation( angle, glm::vec3(x, y, z));
			}

			void scale(float x, float y, float z) {
				transform.setLocalScale(  glm::vec3(x, y, z) );
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