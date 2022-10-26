#pragma once

#ifndef SK_TRANSFORM_
#define SK_TRANSFORM_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const glm::vec3 XVector = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 YVector = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 ZVector = glm::vec3(0.0f, 0.0f, 1.0f);

namespace SKEngio {

	class Transform {
	protected:

		glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f );
		glm::vec3 rotation = glm::vec3( 0.0f, 0.0f, 0.0f );
		glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );

		glm::mat4 modelMatrix = glm::mat4(1.0f);

		bool dirty = true;

	protected:

		glm::mat4 getLocalModelMatrix()
		{
			const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			// Y * X * Z
			const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

			// translation * rotation * scale (also know as TRS matrix)
			return glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
		}

	public:

		void computeModelMatrix()
		{
			modelMatrix = getLocalModelMatrix();
		}

		void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
		{
			modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
		}

		void resetTransforms() {
			position = glm::vec3(0.0f, 0.0f, 0.0f);
			rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			scale = glm::vec3(1.0f, 1.0f, 1.0f);
			modelMatrix = glm::mat4(1.0f);
			dirty = true;
		}


		void setLocalPosition(const glm::vec3& newPosition)
		{
			position = newPosition;
			dirty = true;
		}

		void setLocalRotation(const glm::vec3& newRotation)
		{
			rotation = newRotation;
			dirty = true;
		}

		void setLocalRotation(const float angle, const glm::vec3& axes)
		{
			rotation = angle * axes;
			dirty = true;
		}

		void setLocalScale(const glm::vec3& newScale)
		{
			scale = newScale;
			dirty = true;
		}

		const glm::vec3 getGlobalPosition() const
		{
			return glm::vec3( modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
		}

		const glm::vec3 getLocalPosition() const
		{
			return position;
		}


		const glm::vec3 getLocalRotation() const
		{
			return rotation;
		}

		const glm::vec3 getLocalScale() const
		{
			return scale;
		}

		const glm::mat4 getModelMatrix() const
		{
			return modelMatrix;
		}

		glm::vec3 getRight() const
		{
			return glm::vec3(  modelMatrix[0] );
		}

		glm::vec3 getUp() const
		{
			return glm::vec3( modelMatrix[1] );
		}

		glm::vec3 getBackward() const
		{
			return glm::vec3( modelMatrix[2] );
		}

		glm::vec3 getForward() const
		{
			return glm::vec3(  -modelMatrix[2] );
		}

		bool isDirty() {
			return dirty;
		}

	};
}

#endif
