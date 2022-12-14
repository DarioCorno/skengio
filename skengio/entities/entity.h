#pragma once
#ifndef SK_ENTITY_
#define SK_ENTITY_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <skengio/material.h>
#include <skengio/renderParams.h>
#include "geometries/mesh.h"
//#include "skengio/utils/transform.h"
#include "skengio/entities/transform3d.h"
#include "skengio/entities/geometries/box.h"
#include "skengio/entities/geometries/arrow.h"
#include "skengio/materialsManager.h"

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

			Entity()
		    {
				castShadows = true;
				displayType = EntityDisplayType::Drawable;
				material = MaterialsManager::get().NewMaterial();
			}

			~Entity() {
				delete mesh;
				MaterialsManager::get().DestroyMaterial(material);
			}

			void SetID(std::string _id) {
				id = _id;
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
					gizmo->material->bind(); //->shader->bind();
					ShaderProgram* sp = gizmo->material->GetShader();
					sp->SetCameraUniforms(rp->camera);
					sp->SetModelUniforms(transform.getModelMatrix());
					sp->SetVec3("utilityColor", color);
					gizmo->mesh->draw();
					gizmo->material->unbind();
				}
			}

			void render(RenderParams* rp) {
				ShaderProgram* matShader = material->GetShader();

				if (displayType == EntityDisplayType::Invisible)
					return;

				if ( (rp->pass == RenderPass::ShadowDepth) && (!castShadows) )
					return;

				if (rp->pass == RenderPass::Final) {
					material->bind();

					matShader->SetCameraUniforms(rp->camera);
					matShader->SetModelUniforms(transform.getModelMatrix());
				}
				else {
					//a different shader from this entity shader, used for different render passes
					rp->passShader->SetModelUniforms(transform.getModelMatrix());
				}
				
				mesh->draw();

				if (rp->pass == RenderPass::Final) {
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
				transform.reset();
			}

			void setPosition(float x, float y, float z) {
				transform.setPosition(x, y, z);
			}

			void translate(float x, float y, float z) {
				transform.translate( glm::vec3(x, y, z));
			}

			void rotate(float x, float y, float z) {
				transform.rotate( x, y, z );
			}

			void scale(float x, float y, float z) {
				transform.setScale(  glm::vec3(x, y, z) );
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
				gizmo = new Entity();
				gizmo->SetID("gizmo");
				//gizmo->mesh = new Box();
				//((SKEngio::Box*)gizmo->mesh)->Generate(0.5f, 0.5f, 3.0f, 1, 1, 1);
				gizmo->mesh = new Arrow();
				((SKEngio::Arrow*)gizmo->mesh)->GenerateZ();
				gizmo->mesh->createGLBuffers();
				gizmo->castShadows = false;
				gShader->SetVec3("utilityColor", glm::vec3(1.0f, 1.0f, 1.0f));
				gizmo->material->SetShader(gShader);
				hasGizmo = true;
			}

			std::string id;

			Mesh* mesh{};
			//ShaderProgram* shader{};
			Transform3D transform;
			Material* material;
			bool castShadows;
			EntityDisplayType displayType = EntityDisplayType::Drawable;
			EntityUpdateStatus updateStatus = EntityUpdateStatus::Running;


		private:
			Entity* parent;
			std::vector<Entity*> childs;

			bool hasGizmo = false;
			Entity* gizmo;

	};
}

#endif