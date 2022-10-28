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
#include "skengio/entities/geometries/box.h"

namespace SKEngio {

	enum class EntityUpdateStatus {
		Stopped,
		Running,
		Paused
	};

	enum class EntityDisplayType {
		Drawable,
		Invisible
	};

	class Entity {
		public:

			Entity() : 
				material {new Material()},
				castsShadows {true},
				displayType {EntityDisplayType::Drawable}
		    {
			}

			~Entity() {
				delete mesh;
				delete shader;
				delete material;
				delete cubemap;
			}

			void OnUpdate() {

			}

			void OnDetach() {

			}

			void setInvisible() {
				displayType = EntityDisplayType::Invisible;
			}

			void setVisible() {
				displayType = EntityDisplayType::Drawable;
			}

			//Update transform if it was changed
			void updateSelfAndChild()
			{
				if (updateStatus != EntityUpdateStatus::Running)
					return;

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

			void renderGizmo(RenderParams* rp, glm::vec3 color) {
				if (hasGizmo) {
					gizmo->shader->bind();
					gizmo->shader->SetCameraUniforms(rp->camera);
					gizmo->shader->SetModelUniforms(transform.getModelMatrix());
					gizmo->shader->SetVec3("utilityColor", color);
					gizmo->mesh->draw();
					gizmo->shader->unbind();
				}
			}

			void render(RenderParams* rp) {

				if (displayType == EntityDisplayType::Invisible)
					return;

				if ( (rp->pass == RenderPass::ShadowDepth) && (!castsShadows) )
					return;

				if (rp->pass == RenderPass::Final) {
					shader->bind();

					if (material->diffuseTexture) {
						//useless as the textureId is in the shader material->diffuseTexture->bind();
						shader->SetDiffTexture(material->diffuseTexture->textureUnit);
					}

					if (cubemap != nullptr) {
						//cubemap->bind();
						shader->SetCubeTexture(cubemap->textureUnit);
					}

					shader->SetCameraUniforms(rp->camera);
					shader->SetModelUniforms(transform.getModelMatrix());
					//to correctly render the shadows the entity shader needs to know where the light is
					// see https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
					//shader->SetLightUniforms(rp->light->GetPosition(), rp->light->GetDiffuse(), rp->light->getLightViewProjMatrix());
				}
				else {
					//a different shader from this entity shader, used for "effects"
					rp->passShader->SetModelUniforms(transform.getModelMatrix());
				}
				
				mesh->draw();

				if (rp->pass == RenderPass::Final) {
					//if (material->diffuseTexture)
					//	material->diffuseTexture->unbind();

					renderGizmo(rp, glm::vec3(1.0f, 1.0f, 1.0f) );
				}

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

			void initGizmo(ShaderProgram* gShader) {
				//should all this be in Entity?
				gizmo = new Entity();
				gizmo->mesh = new Box();
				((SKEngio::Box*)gizmo->mesh)->Generate(1.0f, 1.0f, 1.0f, 1, 1, 1);
				gizmo->mesh->buildInterleavedArray();
				gizmo->mesh->createGLBuffers();
				gizmo->shader = gShader;
				gizmo->shader->SetVec3("utilityColor", glm::vec3(1.0f, 1.0f, 1.0f) );	//set a default color
				hasGizmo = true;
			}

			Mesh* mesh{};
			ShaderProgram* shader{};
			Transform transform;
			Material* material;
			bool castsShadows;
			EntityDisplayType displayType = EntityDisplayType::Drawable;
			EntityUpdateStatus updateStatus = EntityUpdateStatus::Running;


		private:

			Entity* parent;
			std::vector<Entity*> childs;
			Texture* cubemap{};

			bool hasGizmo = false;
			Entity* gizmo;

	};
}

#endif